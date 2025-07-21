#include "EnginePCH.h"
#include "PhysicsSystem.h"
#include "Object.h"
#include "Scene.h"
#include "Collider.h"
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

	CurrCollisions.clear();

	Dynamic_vs_Dynamic();
	Dynamic_vs_Static();
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
		if (body->IsKinematic())
			continue;
		//if (body->IsGrounded())
		//	continue;

		body->SetGround(false);

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

void PhysicsSystem::Dynamic_vs_Dynamic()
{
	AABBEntries_Dynamics.clear();

	//Dynamic vs Dynamic
	for (const auto& body : DynamicBodies)
	{
		auto collision = body->GetOwner()->GetComponent<CollisionComponent>();
		auto collider = collision->GetCollider();

		AABB worldAABB = collider->GetWorldAABB();
	
		_float minX, maxX;
		minX = (worldAABB.center - worldAABB.half).x;
		maxX = (worldAABB.center + worldAABB.half).x;

		AABBEntries_Dynamics.push_back({ collision,minX,maxX });
	}

	std::sort(AABBEntries_Dynamics.begin(), AABBEntries_Dynamics.end(), [](const AABBEntry& a, const AABBEntry& b)
		{
			return a.minX < b.minX;
		});
}

void PhysicsSystem::Dynamic_vs_Static()
{
	//Static vs Dynamic
	for (const auto& body : DynamicBodies)
	{
		auto a = body->GetOwner()->GetComponent<CollisionComponent>();
		AABB worldAABB = a->GetCollider()->GetWorldAABB();

		_vec3 worldMin = worldAABB.center - worldAABB.half;
		_vec3 worldMax = worldAABB.center + worldAABB.half;

		int minX, maxX;
		int minY, maxY;
		int minZ, maxZ;

		minX = Grid->WorldToCell(worldMin.x);
		minY = Grid->WorldToCell(worldMin.y);
		minZ = Grid->WorldToCell(worldMin.z);
		maxX = Grid->WorldToCell(worldMax.x);
		maxY = Grid->WorldToCell(worldMax.y);
		maxZ = Grid->WorldToCell(worldMax.z);

		for (int cz = minZ; cz <= maxZ; ++cz)
			for (int cy = minY; cy <= maxY; ++cy)
				for (int cx = minX; cx <= maxX; ++cx)
				{
					const auto& c = Grid->QueryCell(cx, cy, cz);
					if (!c) continue;

					AABB blockAABB = c->GetCollider()->GetWorldAABB();

					_float min = (blockAABB.center - blockAABB.half).x;
					_float max = (blockAABB.center + blockAABB.half).x;
					
					AABBEntries_Statics.push_back({ c,min,max });
				}

		for (int i = 0; i < AABBEntries_Statics.size();++i)
		{
			CollisionComponent* b = AABBEntries_Statics[i].comp;

			if (a->CanCollision(b) && b->CanCollision(a))
			{
				if (a->CheckCollision(b))
					CurrCollisions.emplace(a, b);
			}
		}

		AABBEntries_Statics.clear();
	}
}

void PhysicsSystem::BroadPhase()
{
	for (_uint i = 0; i < AABBEntries_Dynamics.size(); ++i)
	{
		CollisionComponent* a = AABBEntries_Dynamics[i].comp;
		for (_uint j = i + 1; j < AABBEntries_Dynamics.size();++j)
		{
			CollisionComponent* b = AABBEntries_Dynamics[j].comp;

			if (AABBEntries_Dynamics[j].minX > AABBEntries_Dynamics[i].maxX)
				break;

			if (a->CanCollision(b) && b->CanCollision(a))
			{
				if (a->CheckCollision(b))
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

		AABB aWorldAABB = a->GetCollider()->GetWorldAABB();
		AABB bWorldAABB = b->GetCollider()->GetWorldAABB();

		aMin = aWorldAABB.center - aWorldAABB.half;
		aMax = aWorldAABB.center + aWorldAABB.half;

		bMin = bWorldAABB.center - bWorldAABB.half;
		bMax = bWorldAABB.center + bWorldAABB.half;

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
