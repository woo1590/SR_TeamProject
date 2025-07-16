#include "pch.h"
#include "IsRandomMoveCheck.h"
#include "Ender.h"
#include "Object.h"

IsRandomMoveCheck::IsRandomMoveCheck(BTNode* child)
	:DecoratorNode(child)
{
}

IsRandomMoveCheck::~IsRandomMoveCheck()
{
}

BTStatus IsRandomMoveCheck::Initialize(float dt)
{
	return BTStatus();
}

BTStatus IsRandomMoveCheck::Tick(float dt, BlackBoard* bb)
{
	//if (bb == nullptr) return BTStatus::Failure;

	//Object* self = static_cast<Object*>(bb->GetValue("Self"));

	//if (self == nullptr) return BTStatus::Failure;

	//EnderState state = static_cast<Ender*>(self)->GetState();

	//if (state == EnderState::CrawlToStand || state == EnderState::StandToCrawl)
	return BTStatus::Running;

	//else
	//	return Child->Tick(dt, bb);
}

void IsRandomMoveCheck::Free()
{
	DecoratorNode::Free();
}
