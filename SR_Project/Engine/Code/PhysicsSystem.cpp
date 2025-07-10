#include "EnginePCH.h"
#include "PhysicsSystem.h"
#include "Object.h"
#include "Scene.h"
#include "StaticGrid.h"

#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

PhysicsSystem::PhysicsSystem(Scene* owner)
{
	this->owner = owner;
}

PhysicsSystem::~PhysicsSystem()
{
}

PhysicsSystem* PhysicsSystem::Create(Scene* owner)
{
	PhysicsSystem* Instance = new PhysicsSystem(owner);

	if (FAILED(Instance->Ready_PhysicsSystem()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT PhysicsSystem::Ready_PhysicsSystem()
{
	Grid = owner->GetStaticGrid();
	Grid->AddRef();

	return S_OK;
}

void PhysicsSystem::Update(_float dt)
{
	ApplyGravity(dt);
	ApplyVelocity(dt);
	CollectAABBEntry();
	BroadPhase();
	SolvePosition();
	CollisionEvent();
}

void PhysicsSystem::RegisterBody(PhysicsComponent* body)
{
	DynamicBodies.push_back(body);
}

void PhysicsSystem::UnRegisterBody(PhysicsComponent* body)
{
	for (auto iter = DynamicBodies.begin(); iter != DynamicBodies.end(); ++iter)
	{
		if ((*iter) == body)
		{
			DynamicBodies.erase(iter);
			break;
		}
	}
}

void PhysicsSystem::ApplyGravity(_float dt)
{
	for (auto& body : DynamicBodies)
	{
		if (!body->GetMass())
			continue;

		if (body->IsGrounded())
			continue;

		if (body->IsKinematic())
			continue;

		_vec3 velocity = body->GetVelocity();

		velocity.y -= GRAVITY * dt;
		body->SetVelocity(velocity);
	}
}

void PhysicsSystem::ApplyVelocity(_float dt)
{
	for (auto& body : DynamicBodies)
	{
		_vec3 velocity = body->GetVelocity();
		body->GetOwner()->GetComponent<TransformComponent>()->Translate(velocity * dt);
	}
}

void PhysicsSystem::CollectAABBEntry()
{
	AABBEntries.clear();
	std::set<CollisionComponent*> chache;

	//Dynamic vs Dynamic
	for (const auto& body : DynamicBodies)
	{
		auto collision = body->GetOwner()->GetComponent<CollisionComponent>();
		_float minX, maxX;
		collision->GetWorldX(&minX, &maxX);

		AABBEntries.push_back({ collision,minX,maxX });
	}

	//Static vs Dynamic
	for (const auto& body : DynamicBodies)
	{
		auto collision = body->GetOwner()->GetComponent<CollisionComponent>();
		_vec3 worldMin, worldMax;
 		collision->GetWorldAABB(&worldMin, &worldMax);

		int minX, maxX;
		int minY, maxY;
		int minZ, maxZ;

		minX = Grid->WorldToCell(worldMin.x);
		minY = Grid->WorldToCell(worldMin.y);
		minZ = Grid->WorldToCell(worldMin.z);
		maxX = Grid->WorldToCell(worldMax.x);
		maxY = Grid->WorldToCell(worldMax.y);
		maxZ = Grid->WorldToCell(worldMax.z);

		for(int cz = minZ; cz<=maxZ; ++cz)
			for(int cy = minY; cy<=maxY; ++cy)
				for (int cx = minX; cx <= maxX; ++cx)
				{
					const auto& c = Grid->QueryCell(cx, cy, cz);
					if (!c) continue;

					if (!chache.insert(c).second) continue;	//Already Exist

					_float min, max;
					c->GetWorldX(&min, &max);

					AABBEntries.push_back({ c,min,max });
					collision->GetWorldX(&min, &max);
					AABBEntries.push_back({ collision,min,max });
				}
	}

	std::sort(AABBEntries.begin(), AABBEntries.end(), [](AABBEntry& a, AABBEntry& b) {
		return a.minX < b.minX;
		});
}

void PhysicsSystem::BroadPhase()
{
	CurrCollisions.clear();

	for (_uint i = 0; i < AABBEntries.size(); ++i)
	{
		CollisionComponent* a = AABBEntries[i].comp;
		for (_uint j = i + 1; j < AABBEntries.size();++j)
		{
			CollisionComponent* b = AABBEntries[j].comp;

			if (AABBEntries[j].minX > AABBEntries[i].maxX)
				break;

			if (a->CanCollision(b) && b->CanCollision(a))
			{
				if (a->CheckAABBCollision(b))
					CurrCollisions.emplace(a, b);
			}
		}
	}
}

void PhysicsSystem::SolvePosition()
{
	for (auto& pair : CurrCollisions)
	{
		CollisionComponent* a = pair.a;
		CollisionComponent* b = pair.b;

		_vec3 aMin, aMax, bMin, bMax;

		a->GetWorldAABB(&aMin, &aMax);
		b->GetWorldAABB(&bMin, &bMax);

		_float overlapX = (std::min)(aMax.x, bMax.x) - (std::max)(aMin.x, bMin.x);
		_float overlapY = (std::min)(aMax.y, bMax.y) - (std::max)(aMin.y, bMin.y);
		_float overlapZ = (std::min)(aMax.z, bMax.z) - (std::max)(aMin.z, bMin.z);

		_vec3 normal;
		_float pen;

		if (overlapX <= overlapY && overlapX <= overlapZ)
		{
			pen = overlapX;
			normal = (aMin.x < bMin.x) ? _vec3(-1.f, 0.f, 0.f) : _vec3(1.f, 0.f, 0.f);
		}
		else if (overlapY <= overlapX && overlapY <= overlapZ)
		{
			pen = overlapY;
			normal = (aMin.y < bMin.y) ? _vec3(0.f, -1.f, 0.f) : _vec3(0.f, 1.f, 0.f);
		}
		else
		{
			pen = overlapZ;
			normal = (aMin.z < bMin.z) ? _vec3(0.f, 0., -1.f) : _vec3(0.f, 0.f, 1.f);
		}

		_vec3 mtv = normal * pen;

		auto physicsA = a->GetOwner()->GetComponent<PhysicsComponent>();
		auto physicsB = b->GetOwner()->GetComponent<PhysicsComponent>();

		if (!physicsA || !physicsB)
			continue;

		if (physicsA->IsKinematic() || physicsB->IsKinematic())
			continue;

		_float invA = physicsA->GetInvMass();
		_float invB = physicsB->GetInvMass();
		_float sum = invA + invB;

		if (!sum) continue;

		a->GetOwner()->GetComponent<TransformComponent>()->Translate(mtv * (invA / sum));
		b->GetOwner()->GetComponent<TransformComponent>()->Translate(-mtv * (invB / sum));

		physicsA->SetGround(false);
		physicsB->SetGround(false);

		if (normal.y > 0.7f)
		{
			_vec3 velocity = physicsA->GetVelocity();
			velocity.y = 0.f;
			physicsA->SetVelocity(velocity);
			physicsA->SetGround(true);
		}
		else if (normal.y < -0.7f)
		{
			_vec3 velocity = physicsB->GetVelocity();
			velocity.y = 0.f;
			physicsB->SetVelocity(velocity);
			physicsB->SetGround(true);
		}
	}
}

void PhysicsSystem::CollisionEvent()
{
	//Collision Enter
	for (const auto& pair : CurrCollisions)
	{
		if (!PrevCollisions.count(pair))
		{
			pair.a->OnCollisionEnter(pair.b);
			pair.b->OnCollisionEnter(pair.a);
		}
	}

	//Collision Stay
	for (const auto& pair : CurrCollisions)
	{
		if (PrevCollisions.count(pair))
		{
			pair.a->OnCollisionStay(pair.b);
			pair.b->OnCollisionStay(pair.a);
		}
	}

	//Collision Exit
	for (const auto& pair : PrevCollisions)
	{
		if (!CurrCollisions.count(pair))
		{
			pair.a->OnCollisionExit(pair.b);
			pair.b->OnCollisionExit(pair.a);
		}
	}

	PrevCollisions.swap(CurrCollisions);
}

void PhysicsSystem::Free()
{
	Safe_Release(Grid);
}
