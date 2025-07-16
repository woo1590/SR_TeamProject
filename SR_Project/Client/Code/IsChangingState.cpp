#include "pch.h"
#include "IsChangingState.h"
#include "Object.h"
#include "Ender.h"

IsChangingState::IsChangingState(BTNode* child)
	:DecoratorNode(child)
{
}

IsChangingState::~IsChangingState()
{
}

BTStatus IsChangingState::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus IsChangingState::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));

	if (self == nullptr) return BTStatus::Failure;
	
	EnderState state = static_cast<Ender*>(self)->GetState();
	
	if (state == EnderState::CrawlToStand || state == EnderState::StandToCrawl)
		return BTStatus::Running;

	else
		return Child->Tick(dt, bb);
}

void IsChangingState::Free()
{
	DecoratorNode::Free();
}
