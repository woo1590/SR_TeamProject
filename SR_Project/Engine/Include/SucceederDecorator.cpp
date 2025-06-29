#include "EnginePCH.h"
#include "SucceederDecorator.h"

SucceederDecorator::SucceederDecorator(BTNode* child)
	:DecoratorNode(child)
{
}

SucceederDecorator::~SucceederDecorator()
{
}

BTStatus SucceederDecorator::Initialize(float dt)
{
	if (Child)
		return Child->Initialize(dt);
	return BTStatus::Failure;
}

BTStatus SucceederDecorator::Tick(float dt, BlackBoard* bb)
{
    if (!Child)
        return BTStatus::Failure;

    BTStatus status = Child->Tick(dt, bb);

    switch (status)
    {
    case BTStatus::Success:
        return BTStatus::Success;

    case BTStatus::Failure:
        return BTStatus::Success;

    case BTStatus::Running:
        return BTStatus::Running;
    }

    return BTStatus::Success;
}

void SucceederDecorator::Free()
{
    DecoratorNode::Free();
}
