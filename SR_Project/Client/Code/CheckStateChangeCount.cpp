#include "pch.h"
#include "CheckStateChangeCount.h"
#include "Object.h"
#include "Ender.h"

CheckStateChangeCount::CheckStateChangeCount(BTNode* child)
	:DecoratorNode(child)
{
}

CheckStateChangeCount::~CheckStateChangeCount()
{
}

BTStatus CheckStateChangeCount::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus CheckStateChangeCount::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	int* triggercount = static_cast<int*>(bb->GetValue("TriggerCount"));
	
	if (self == nullptr) return BTStatus::Failure;

	int count = static_cast<Ender*>(self)->GetCurChangeStateCount();

	if (count < *triggercount)
		return BTStatus::Failure;

	else
		return Child->Tick(dt, bb);
}

void CheckStateChangeCount::Free()
{
	DecoratorNode::Free();
}
