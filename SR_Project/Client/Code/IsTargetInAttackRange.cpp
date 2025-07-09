#include "pch.h"
#include "IsTargetInAttackRange.h"
#include "Object.h"
#include "TransformComponent.h"

IsTargetInAttackRange::IsTargetInAttackRange(BTNode* child)
	:DecoratorNode(child)
{
}

IsTargetInAttackRange::~IsTargetInAttackRange()
{
}

BTStatus IsTargetInAttackRange::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus IsTargetInAttackRange::Tick(float dt, BlackBoard* bb)
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
		return Child->Tick(dt, bb);

	if (D3DXVec3Length(&Axis) < *(static_cast<float*>(bb->GetValue("Distance"))))
		return Child->Tick(dt, bb);
	else
		return BTStatus::Failure;
}

void IsTargetInAttackRange::Free()
{
	DecoratorNode::Free();
}
