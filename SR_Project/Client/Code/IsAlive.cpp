#include "pch.h"
#include "IsAlive.h"
#include "Monster.h"
#include "Object.h"
#include "InfoComponent.h"

IsAliveNode::IsAliveNode(BTNode* child)
	:DecoratorNode(child)
{
}

IsAliveNode::~IsAliveNode()
{
}

BTStatus IsAliveNode::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus IsAliveNode::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	_bool* IsHit = static_cast<_bool*>(bb->GetValue("IsDamaged"));

	if (self == nullptr) return BTStatus::Failure;

	auto stat = self->GetComponent<InfoComponent<EnemyInfo>>();

	if (stat->GetInfo().curHp > 0)
	{
		if (IsHit == nullptr ||
			(IsHit != nullptr && (*IsHit) == false))
			return Child->Tick(dt, bb);
		else return BTStatus::Running;
	}
	else
		return BTStatus::Failure;
}

void IsAliveNode::Free()
{
	DecoratorNode::Free();
}
