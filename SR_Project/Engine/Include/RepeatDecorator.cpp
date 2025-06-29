#include "EnginePCH.h"
#include "RepeatDecorator.h"

RepeatDecorator::RepeatDecorator(BTNode* child)
    :DecoratorNode(child)
{
}

RepeatDecorator::~RepeatDecorator()
{
}

BTStatus RepeatDecorator::Initialize(float dt)
{
    CurrentCnt = 0;

	if (Child)
		return Child->Initialize(dt);
	return BTStatus::Failure;
}

BTStatus RepeatDecorator::Tick(float dt, BlackBoard* bb)
{
    if (!Child)
        return BTStatus::Failure;

    BTStatus status = Child->Tick(dt, bb);

    switch (status)
    {
    case BTStatus::Success:
    case BTStatus::Failure:
        ++CurrentCnt;
        break;

    case BTStatus::Running:
        return BTStatus::Running;
    }

    if (CurrentCnt >= RepeatCnt)
    {
        CurrentCnt = 0;
        return BTStatus::Success;
    }

    Child->Initialize(dt);
    return BTStatus::Running;
}

void RepeatDecorator::Free()
{
    DecoratorNode::Free();
}
