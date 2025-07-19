#include "pch.h"
#include "ChangeState.h"
#include "Object.h"
#include "Ender.h"
#include "TransformComponent.h"

ChangeStateNode::ChangeStateNode()
{
}

ChangeStateNode::~ChangeStateNode()
{
}

BTStatus ChangeStateNode::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus ChangeStateNode::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	_vec3* targetPos = static_cast<_vec3*>(bb->GetValue("targetPos"));

	if (self == nullptr) return BTStatus::Failure;
	Ender* ender = static_cast<Ender*>(self);

	auto transform = self->GetComponent<TransformComponent>();
	_vec3 pos = transform->GetPosition();

	int randx = rand() % 10 + 10;
	int randz = rand() % 10 + 10;

	if (rand() % 2) randx *= -1;
	if (rand() % 2) randz *= -1;

	randx += pos.x;
	randz += pos.z;

	switch (ender->GetState())
	{
	case EnderState::Crawl:
		ender->CrawlToStand();
		*targetPos = _vec3(randx, 0.f, randz);
		return BTStatus::Success;

	case EnderState::Hidden:
		ender->Sprout();
		*targetPos = _vec3(randx, 0.f, randz);
		return BTStatus::Success;

	case EnderState::Stand:
	{
		switch (rand() % 2)
		{
		case 0:
			ender->StandToCrawl();
			*targetPos = _vec3(randx, 0.f, randz);
			return BTStatus::Success;

		case 1:
			ender->Hide();
			*targetPos = _vec3(randx, 0.f, randz);
			return BTStatus::Success;
		}
	}
	}
}

void ChangeStateNode::Free()
{
	ActionNode::Free();
}
