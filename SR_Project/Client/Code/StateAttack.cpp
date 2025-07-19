#include "pch.h"
#include "StateAttack.h"
#include "Object.h"
#include "Ender.h"
#include "TransformComponent.h"
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
	_vec3* targetPos = static_cast<_vec3*>(bb->GetValue("targetPos"));

	if (self == nullptr) return BTStatus::Failure;
	Ender* ender = static_cast<Ender*>(self);

	if (*Attack) return BTStatus::Running;
	if (*Attack && ender->IsAttackFinish())
	{
		auto transform = self->GetComponent<TransformComponent>();
		_vec3 pos = transform->GetPosition();
		int randx = rand() % 10 + 15;
		int randz = rand() % 10 + 15;

		if (rand() % 2) randx *= -1;
		if (rand() % 2) randz *= -1;

		randx += pos.x;
		randz += pos.z;
		*targetPos = _vec3(randx, pos.y, randz);
		return BTStatus::Success;
	}

	switch (ender->GetState())
	{
	case EnderState::Crawl:
	{
		ender->CrawlToStand();
		auto transform = self->GetComponent<TransformComponent>();
		_vec3 pos = transform->GetPosition();
		int randx = rand() % 10 + 15;
		int randz = rand() % 10 + 15;

		if (rand() % 2) randx *= -1;
		if (rand() % 2) randz *= -1;

		randx += pos.x;
		randz += pos.z;
		*targetPos = _vec3(randx, pos.y, randz);
		return BTStatus::Success;
	}
	case EnderState::HideIdle:
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
