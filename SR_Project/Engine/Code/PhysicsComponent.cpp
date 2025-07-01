#include "EnginePCH.h"
#include "PhysicsComponent.h"

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

void PhysicsComponent::Update(_float dt)
{
	if (Is_Ground)
		Velocity = { 0.f,0.f,0.f };
	else
		Velocity.y -= GRAVITY * dt;
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
