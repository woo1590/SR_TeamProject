#include "pch.h"
#include "Hit.h"
#include "Object.h"
#include "Monster.h"
#include "InfoComponent.h"
#include "TransformComponent.h"
#include "InfoDetector.h"

#include "EngineCore.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "TestScene.h"

#include "HPBarFront.h"
#include "ProgressBar.h"
HitNode::HitNode()
{
}

HitNode::~HitNode()
{
}

BTStatus HitNode::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus HitNode::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr)
		return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	Object* target = static_cast<Object*>(bb->GetValue("Target"));
	
	if (self == nullptr) return BTStatus::Failure;
	auto monster = static_cast<Monster*>(self);

	auto stat = self->GetComponent<InfoComponent<EnemyInfo>>();
	auto playerStat = target->GetComponent<InfoComponent<PlayerInfo>>();

	auto selfTransform = self->GetComponent<TransformComponent>();
	auto playerTransform = target->GetComponent<TransformComponent>();

	if (!monster->IsHitStart())
	{
		//monster->Hit(selfTransform->GetPosition() - playerTransform->GetPosition(), playerStat->GetInfo().power, dt);
		return BTStatus::Running;
	}
	if (monster->IsHitStart() && !monster->IsHitFinish())
	{
		//monster->Hit(selfTransform->GetPosition() - playerTransform->GetPosition(), playerStat->GetInfo().power, dt);
		return BTStatus::Running;
	}
	else if (!monster->IsHitStart() && monster->IsHitFinish())
	{
		return BTStatus::Success;
	}
	return BTStatus::Failure;
}

void HitNode::Free()
{
	ActionNode::Free();
}
