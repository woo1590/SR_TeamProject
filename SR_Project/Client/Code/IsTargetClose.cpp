#include "pch.h"
#include "IsTargetClose.h"
#include "Object.h"
#include "TransformComponent.h"

IsTargetClose::IsTargetClose(BTNode* child)
	:DecoratorNode(child)
{
}

IsTargetClose::~IsTargetClose()
{
}

BTStatus IsTargetClose::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus IsTargetClose::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	Object* target = static_cast<Object*>(bb->GetValue("Target"));
	_bool* IsAttack = static_cast<_bool*>(bb->GetValue("IsAttack"));

	if (self == nullptr || target == nullptr) return BTStatus::Failure;

	_vec3 TargetPos = target->GetComponent<TransformComponent>()->GetPosition();
	_vec3 SelfPos = self->GetComponent<TransformComponent>()->GetPosition();

	_vec3 Axis = TargetPos - SelfPos;

	if (IsAttack != nullptr && *IsAttack == true)
		return BTStatus::Failure;

	if (D3DXVec3Length(&Axis) < *(static_cast<float*>(bb->GetValue("NearDistance"))))
		return Child->Tick(dt, bb);
	else
		return BTStatus::Failure;
}

void IsTargetClose::Free()
{
	DecoratorNode::Free();
}
