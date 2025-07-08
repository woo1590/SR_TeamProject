#include "pch.h"
#include "IsTimerOn.h"

IsTimerOn::IsTimerOn(BTNode* child)
	:DecoratorNode(child)
{
}

IsTimerOn::~IsTimerOn()
{
}

BTStatus IsTimerOn::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus IsTimerOn::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr)
		return BTStatus::Failure;

	float* Timer = static_cast<float*>(bb->GetValue("Timer"));

	if (Timer == nullptr) return BTStatus::Failure;

	if (*Timer < 0)
		return Child->Tick(dt, bb);

	else
		return BTStatus::Failure;
}

void IsTimerOn::Free()
{
	DecoratorNode::Free();
}