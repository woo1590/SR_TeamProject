#include "EnginePCH.h"
#include "PhysicsSystem.h"
#include "Object.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

PhysicsSystem::PhysicsSystem(Scene* owner)
{
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
	return S_OK;
}

void PhysicsSystem::Update(_float dt)
{
	ApplyGravity(dt);
	ApplyVelocity(dt);
	SortAABBEntry();
	BroadPhase();
	SolvePosition();
	CollisionEvent();
}

void PhysicsSystem::RegisterBody(PhysicsComponent* body)
{
	Bodies.push_back(body);
}

void PhysicsSystem::RegisterCollision(CollisionComponent* collision)
{
	Collisions.push_back(collision);
}

void PhysicsSystem::ApplyGravity(_float dt)
{
	for (auto& body : Bodies)
	{
		if (!body->GetMass())
			continue;

		if (body->IsGrounded())
			continue;

		_vec3 velocity = body->GetVelocity();

		velocity.y -= GRAVITY * dt;
		body->SetVelocity(velocity);
	}
}

void PhysicsSystem::ApplyVelocity(_float dt)
{
	for (auto& body : Bodies)
	{
		_vec3 velocity = body->GetVelocity();
		body->GetOwner()->GetComponent<TransformComponent>()->Translate(velocity * dt);
	}
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

		_float invA = a->GetOwner()->GetComponent<PhysicsComponent>()->GetInvMass();
		_float invB = b->GetOwner()->GetComponent<PhysicsComponent>()->GetInvMass();
		_float sum = invA + invB;

		if (!sum) continue;

		a->GetOwner()->GetComponent<TransformComponent>()->Translate(mtv * (invA / sum));
		b->GetOwner()->GetComponent<TransformComponent>()->Translate(-mtv * (invB / sum));

		auto physicsA = a->GetOwner()->GetComponent<PhysicsComponent>();
		auto physicsB = b->GetOwner()->GetComponent<PhysicsComponent>();

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

void PhysicsSystem::SortAABBEntry()
{
	AABBEntries.clear();
	AABBEntries.reserve(Collisions.size());

	for (const auto& c : Collisions)
	{
		_vec3 min, max;
		c->GetWorldAABB(&min, &max);

		AABBEntries.emplace_back(AABBEntry{ c,min.x,max.x });
	}

	std::sort(AABBEntries.begin(), AABBEntries.end(), [](AABBEntry& a, AABBEntry& b) {
		return a.minX < b.minX;
		});
}

void PhysicsSystem::Free()
{

}
