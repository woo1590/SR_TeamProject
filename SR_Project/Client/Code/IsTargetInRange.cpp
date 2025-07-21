#include "pch.h"
#include "IsTargetInRange.h"
#include "Object.h"
#include "TransformComponent.h"
#include "Monster.h"

IsTargetInRange::IsTargetInRange(BTNode* child)
	:DecoratorNode(child)
{
}

IsTargetInRange::~IsTargetInRange()
{
}

BTStatus IsTargetInRange::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus IsTargetInRange::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	Object* target = static_cast<Object*>(bb->GetValue("Target"));
	int* AttackNumber = static_cast<int*>(bb->GetValue("AttackNumber"));
	float* Timer = static_cast<float*>(bb->GetValue("Timer"));

	if (self == nullptr || target == nullptr) return BTStatus::Failure;

	_vec3 TargetPos = target->GetComponent<TransformComponent>()->GetPosition();
	_vec3 SelfPos = self->GetComponent<TransformComponent>()->GetPosition();

	_vec3 Axis = TargetPos - SelfPos;

	Monster* monster = static_cast<Monster*>(self);

	if (!monster->IsAttackStart())
	{
		if (D3DXVec3Length(&Axis) > *(static_cast<float*>(bb->GetValue("Distance"))))
			*AttackNumber = 2;
	
		else
		{
			_vec3 forward = self->GetComponent<TransformComponent>()->GetFoward();
			Axis.y = 0.f;
			forward.y = 0.f;

			_vec3 cross;
			D3DXVec3Normalize(&Axis, &Axis);
			D3DXVec3Cross(&cross, &forward, &Axis);
			if (cross.y >= 0.f)
			{
				*AttackNumber = 1;
			}
			else
			{
				*AttackNumber = 3;
			}
		}
		monster->Attack(target);
	}
	if (monster->IsAttackFinish())
	{
		return BTStatus::Failure;
	}
	return BTStatus::Running;
}

void IsTargetInRange::Free()
{
	DecoratorNode::Free();
}