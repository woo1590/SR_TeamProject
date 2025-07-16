#include "pch.h"
#include "ChangeState.h"
#include "Object.h"
#include "Ender.h"

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

	if (self == nullptr) return BTStatus::Failure;
	Ender* ender = static_cast<Ender*>(self);

	switch (ender->GetState())
	{
	case EnderState::Crawl:
		ender->CrawlToStand();
		return BTStatus::Success;
		
	case EnderState::Hidden:
		ender->Sprout();
		return BTStatus::Success;

	case EnderState::Stand:
	{
		switch (rand() % 2)
		{
		case 0:
			ender->StandToCrawl();
			return BTStatus::Success;

		case 1:
			ender->Hide();
			return BTStatus::Success;
		}
	}
	}
}

void ChangeStateNode::Free()
{
	ActionNode::Free();
}
