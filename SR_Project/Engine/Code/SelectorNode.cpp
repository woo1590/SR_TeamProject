#include "EnginePCH.h"
#include "SelectorNode.h"

SelectorNode::SelectorNode()
{
}

SelectorNode::~SelectorNode()
{
}

BTStatus SelectorNode::Initialize(float dt)
{
    CurrentIndex = 0;

    for (auto& child : ChildNodes)
    {
        if (child) child->Initialize(dt);
    }

    return BTStatus::Running;
}

BTStatus SelectorNode::Tick(float dt, BlackBoard* bb)
{
    if (CurrentIndex >= ChildNodes.size())
    {
        CurrentIndex = 0;
        return BTStatus::Failure;
    }
    BTStatus status = ChildNodes[CurrentIndex]->Tick(dt, bb);

    switch (status)
    {
    case BTStatus::Success:
        CurrentIndex = 0;
        return BTStatus::Success;

    case BTStatus::Failure:
        ++CurrentIndex;
        return BTStatus::Running;

    case BTStatus::Running:
        return BTStatus::Running;
    }

	return BTStatus::Failure;
}

void SelectorNode::Free()
{
    CompositeNode::Free();
}
