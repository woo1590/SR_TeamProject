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
	BroadPhase();
	ApplyVelocity(dt);
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

void PhysicsSystem::BroadPhase()
{
	CurrCollision.clear();

	for (_uint i = 0; i < Collisions.size(); ++i)
	{
		CollisionComponent* a = Collisions[i];

		for (_uint j = i + 1; j < Collisions.size(); ++j)
		{
			CollisionComponent* b = Collisions[j];

			if (a->CanCollision(b) && b->CanCollision(a))
			{
				if (a->CheckAABBCollision(b))
				{
					CurrCollision.push_back(std::pair<CollisionComponent*, CollisionComponent*>(a, b));
				}
			}
		}
	}
}

void PhysicsSystem::SolvePosition()
{
	for (auto& pair : CurrCollision)
	{
		CollisionComponent* a = pair.first;
		CollisionComponent* b = pair.second;

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

		_float invA = a->GetOwner()->GetComponent<PhysicsComponent>()->GetMass();
		_float invB = b->GetOwner()->GetComponent<PhysicsComponent>()->GetMass();
		_float sum = invA + invB;

		if (!sum) continue;

		a->GetOwner()->GetComponent<TransformComponent>()->Translate(-mtv * (invA / sum));
		b->GetOwner()->GetComponent<TransformComponent>()->Translate(mtv * (invB / sum));

		if (normal.y > 0.7f)
		{
			auto physics = a->GetOwner()->GetComponent<PhysicsComponent>();

			_vec3 velocity = physics->GetVelocity();
			velocity.y = 0.f;
			physics->SetVelocity(velocity);
			physics->SetGround(true);
		}
		else if (normal.y < -0.7f)
		{
			auto physics = b->GetOwner()->GetComponent<PhysicsComponent>();

			_vec3 velocity = physics->GetVelocity();
			velocity.y = 0.f;
			physics->SetVelocity(velocity);
			physics->SetGround(true);
		}
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

void PhysicsSystem::CollisionEvent()
{
	//Collision Enter
	for (const auto& currPair : CurrCollision)
	{
		currPair.first->OnCollisionEnter(currPair.second);
		currPair.second->OnCollisionEnter(currPair.first);
	}

	//Collision Stay
	for (const auto& prevPair : PrevCollision)
	{
		for (const auto& currPair : CurrCollision)
		{
			auto it = std::find(PrevCollision.begin(), PrevCollision.end(), currPair);
			if (it != PrevCollision.end())
			{
				currPair.first->OnCollisionStay(currPair.second);
				currPair.second->OnCollisionStay(currPair.first);
			}
		}
	}

	//Collision Exit
	for (const auto& prevPair : PrevCollision)
	{
		for (const auto& currPair : CurrCollision)
		{
			auto it = std::find(PrevCollision.begin(), PrevCollision.end(), currPair);
			if (it != PrevCollision.end())
			{
				currPair.first->OnCollisionExit(currPair.second);
				currPair.second->OnCollisionExit(currPair.first);
			}
		}
	}

	PrevCollision.swap(CurrCollision);
}

void PhysicsSystem::Free()
{

}
