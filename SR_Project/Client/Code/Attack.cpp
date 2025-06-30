#include "pch.h"
#include "Attack.h"
#include "Object.h"
#include "Monster.h"

AttackNode::AttackNode()
{
}

AttackNode::~AttackNode()
{
}

BTStatus AttackNode::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus AttackNode::Tick(float dt, BlackBoard* bb)
{
    Object* self = static_cast<Object*>(bb->GetValue("Self"));
    Object* target = static_cast<Object*>(bb->GetValue("Target"));

    if (!self || !target) return BTStatus::Failure;

    static_cast<Monster*>(self)->Attack(target);

    return BTStatus::Success; 
}

void AttackNode::Free()
{
    ActionNode::Free();
}
