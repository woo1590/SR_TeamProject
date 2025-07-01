#include "pch.h"
#include "Die.h"
#include "Object.h"
#include "Monster.h"

DieNode::DieNode()
{
}

DieNode::~DieNode()
{
}

BTStatus DieNode::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus DieNode::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));

	if (self == nullptr) return BTStatus::Failure;

	Monster* monster = static_cast<Monster*>(self);
	monster->Die();

	if (monster->IsDeadStart()) return BTStatus::Running;
	if (monster->IsDeadFinish()) return BTStatus::Success;

	return BTStatus::Failure;
}

void DieNode::Free()
{
	ActionNode::Free();
}
