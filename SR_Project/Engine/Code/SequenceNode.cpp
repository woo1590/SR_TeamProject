#include "EnginePCH.h"
#include "SequenceNode.h"

SequenceNode::SequenceNode()
{
}

SequenceNode::~SequenceNode()
{
}

BTStatus SequenceNode::Initialize(float dt)
{
    CurrentIndex = 0;

    for (auto& child : ChildNodes) 
    {
        if (child) child->Initialize(dt);
    }

    return BTStatus::Running; 
}

BTStatus SequenceNode::Tick(float dt, BlackBoard* bb)
{
    if (CurrentIndex >= ChildNodes.size())
    {
        CurrentIndex = 0;
        return BTStatus::Success;
    }

    BTStatus status = ChildNodes[CurrentIndex]->Tick(dt, bb);

    switch (status)
    {
    case BTStatus::Success:
        ++CurrentIndex;
        return BTStatus::Running;

    case BTStatus::Failure:
        CurrentIndex = 0;
        return BTStatus::Failure;

    case BTStatus::Running:
        return BTStatus::Running;
    }
}

void SequenceNode::Free()
{
    CompositeNode::Free();
}
