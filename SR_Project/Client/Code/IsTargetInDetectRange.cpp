#include "pch.h"
#include "IsTargetInDetectRange.h"

IsTargetInDetectRange::IsTargetInDetectRange(BTNode* child)
	:DecoratorNode(child)
{
}

IsTargetInDetectRange::~IsTargetInDetectRange()
{
}

BTStatus IsTargetInDetectRange::Initialize(float dt)
{
	return BTStatus();
}

BTStatus IsTargetInDetectRange::Tick(float dt, BlackBoard* bb)
{
	return BTStatus();
}

void IsTargetInDetectRange::Free()
{
}
