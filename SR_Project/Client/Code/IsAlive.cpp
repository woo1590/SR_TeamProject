#include "pch.h"
#include "IsAlive.h"
#include "Monster.h"
#include "Object.h"

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

	if (self == nullptr) return BTStatus::Failure;

	//statcomponent chuga ho modify yejeong
	if (/*static_cast<Monster*>(self)->GetHp() > 0*/true)
		return Child->Tick(dt, bb);
	else
		return BTStatus::Failure;
}

void IsAliveNode::Free()
{
	DecoratorNode::Free();
}
