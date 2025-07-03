#include "EnginePCH.h"
#include "PhysicsComponent.h"
#include "Object.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "PhysicsSystem.h"

PhysicsComponent::PhysicsComponent(Object* owner)
	:ObjectComponent(owner)
{
}

PhysicsComponent::~PhysicsComponent()
{
}

PhysicsComponent* PhysicsComponent::Create(Object* owner)
{
	PhysicsComponent* Instance = new PhysicsComponent(owner);

	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT PhysicsComponent::Ready_Component()
{
	//owner->GetScene()->GetPhysicsStstem()->RegisterBody(this);

	return S_OK;
}

void PhysicsComponent::Update(_float dt)
{
	if (Is_Ground)
		Velocity.y = 0.f;
	else
		Velocity.y -= 30.f * dt;
}

void PhysicsComponent::SetVelocity(_vec3 velocity)
{
	Velocity = velocity;
}

void PhysicsComponent::SetVelocity(_float x, _float y, _float z)
{
	Velocity = { x,y,z };
}

_vec3 PhysicsComponent::GetVelocity() const
{
	return Velocity;
}

void PhysicsComponent::SetGround(_bool ground)
{
	Is_Ground = ground;
}

_bool PhysicsComponent::IsGrounded() const
{
	return Is_Ground;
}

void PhysicsComponent::Free()
{
	ObjectComponent::Free();
}
