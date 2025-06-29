#include "EnginePCH.h"
#include "InverterDecorator.h"

InverterDecorator::InverterDecorator(BTNode* child)
    :DecoratorNode(child)
{
}

InverterDecorator::~InverterDecorator()
{
}

BTStatus InverterDecorator::Initialize(float dt)
{
    if (Child)
        return Child->Initialize(dt);
    return BTStatus::Failure;
}

BTStatus InverterDecorator::Tick(float dt, BlackBoard* bb)
{
    if (!Child)
        return BTStatus::Failure;

    BTStatus status = Child->Tick(dt, bb);

    switch (status)
    {
    case BTStatus::Success:
        return BTStatus::Failure;

    case BTStatus::Failure:
        return BTStatus::Success;

    case BTStatus::Running:
        return BTStatus::Running;
    }

    return BTStatus::Failure;
}

void InverterDecorator::Free()
{
    DecoratorNode::Free();
}
