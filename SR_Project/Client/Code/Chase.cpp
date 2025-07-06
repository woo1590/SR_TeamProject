#include "pch.h"
#include "Chase.h"
#include "Object.h"
#include "TransformComponent.h"
#include "BaseCharacter.h"

ChaseNode::ChaseNode()
{
}

ChaseNode::~ChaseNode()
{
}

BTStatus ChaseNode::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus ChaseNode::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	Object* target = static_cast<Object*>(bb->GetValue("Target"));

	if (self == nullptr || target == nullptr) return BTStatus::Failure;
	
	_vec3 TargetPos = target->GetComponent<TransformComponent>()->GetPosition();
	_vec3 SelfPos = self->GetComponent<TransformComponent>()->GetPosition();
	
	_vec3 Dir = TargetPos - SelfPos;

	if (D3DXVec3Length(&Dir) > *(static_cast<float*>(bb->GetValue("Distance"))))
	{
		//Dir.y = 0;
		static_cast<BaseCharacter*>(self)->MoveTo(D3DXVec3Normalize(&Dir, &Dir), dt);
		return BTStatus::Running;
	}
	else
		return BTStatus::Success;

	return BTStatus::Failure;
}

void ChaseNode::Free()
{
	ActionNode::Free();
}
