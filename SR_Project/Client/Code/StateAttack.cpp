#include "pch.h"
#include "StateAttack.h"
#include "Object.h"
#include "Ender.h"

StateAttackNode::StateAttackNode()
{
}

StateAttackNode::~StateAttackNode()
{
}

BTStatus StateAttackNode::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus StateAttackNode::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	_bool* Attack = static_cast<_bool*>(bb->GetValue("IsAttack"));
	if (self == nullptr) return BTStatus::Failure;
	Ender* ender = static_cast<Ender*>(self);

	if (*Attack) return BTStatus::Running;
	if (*Attack && ender->IsAttackFinish()) return BTStatus::Success;

	switch (ender->GetState())
	{
	case EnderState::Crawl:
		//ender->CrawlToStand();
		*Attack = true;
		return BTStatus::Running;

	case EnderState::Hidden:
		ender->LineLaserAttack();
		*Attack = true;
		return BTStatus::Running;

	case EnderState::Stand:
	{
		switch (rand() % 2)
		{
		case 0:
			ender->CrossLaserAttack();
			*Attack = true;
			return BTStatus::Running;

		case 1:
			ender->ProjectileAttack();
			*Attack = true;
			return BTStatus::Running;
		}
	}
	}
}

void StateAttackNode::Free()
{
	ActionNode::Free();
}
