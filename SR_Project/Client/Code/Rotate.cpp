#include "pch.h"
#include "Rotate.h"
#include "Object.h"
#include "Monster.h"
#include "TransformComponent.h"

RotateNode::RotateNode()
{
}

RotateNode::~RotateNode()
{
}

BTStatus RotateNode::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus RotateNode::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	Object* target = static_cast<Object*>(bb->GetValue("Target"));

	if (self == nullptr) return BTStatus::Failure;

	_vec3 TargetPos = target->GetComponent<TransformComponent>()->GetPosition();
	_vec3 SelfPos = self->GetComponent<TransformComponent>()->GetPosition();

	_vec3 Dir = _vec3(TargetPos.x - SelfPos.x, 0.f, TargetPos.z - SelfPos.z);

	Monster* monster = static_cast<Monster*>(self);
	D3DXVec3Normalize(&Dir, &Dir);

	_vec3 forward = _vec3 (self->GetComponent<TransformComponent>()->GetFoward().x, 0.f, self->GetComponent<TransformComponent>()->GetFoward().z);
	D3DXVec3Normalize(&forward, &forward);
 	float dot = D3DXVec3Dot(&Dir, &forward);

	if (dot > 0.99f) 
		return BTStatus::Success;

	else monster->RotateTo(&Dir, dt);

	return BTStatus::Running;
}

void RotateNode::Free()
{
	ActionNode::Free();
}
