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

	_vec3 Dir = TargetPos - SelfPos;

	Monster* monster = static_cast<Monster*>(self);
	D3DXVec3Normalize(&Dir, &Dir);

	_vec3 forward = self->GetComponent<TransformComponent>()->GetFoward();
	D3DXVec3Normalize(&forward, &forward);
	float dot = D3DXVec3Dot(&Dir, &forward); // 둘 다 정규화된 상태여야 함
	if (dot > 0.99f) // 약 8도 이하
		return BTStatus::Success;

	else
		monster->RotateTo(&Dir, dt);

	return BTStatus::Running;

}

void RotateNode::Free()
{
	ActionNode::Free();
}
