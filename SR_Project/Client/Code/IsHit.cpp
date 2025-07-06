#include "pch.h"
#include "IsHit.h"
#include "Monster.h"
#include "Object.h"
#include "InfoComponent.h"

IsHitNode::IsHitNode(BTNode* child)
	:DecoratorNode(child)
{
}

IsHitNode::~IsHitNode()
{
}

BTStatus IsHitNode::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus IsHitNode::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr)
		return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	_bool* IsHit = static_cast<_bool*>(bb->GetValue("IsDamaged"));

	if (self == nullptr || IsHit == nullptr) return BTStatus::Failure;

	if (*IsHit)
	{
		*IsHit = false;
		return Child->Tick(dt, bb);
	}
	else
		return BTStatus::Failure;
}

void IsHitNode::Free()
{
}
