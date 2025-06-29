#include "EnginePCH.h"
#include "ParallelNode.h"

ParallelNode::ParallelNode()
{
}

ParallelNode::~ParallelNode()
{
}

BTStatus ParallelNode::Initialize(float dt)
{
    CurrentIndex = 0;

    for (auto& child : ChildNodes) 
    {
        if (child)
            child->Initialize(dt);
    }

    return BTStatus::Running;
}

BTStatus ParallelNode::Tick(float dt,BlackBoard* bb)
{
    if (ChildNodes.size() == 0) return BTStatus::Failure;
	
    bool HasRunning = false;
    bool HasFail = false;

    for (auto& node : ChildNodes)
    {
        BTStatus status = node->Tick(dt, bb);

        if (BTStatus::Running == status)
            HasRunning = true;

        else if (BTStatus::Failure == status)
            HasFail = true;
    }

    if (HasRunning)
        return BTStatus::Running;

    if (HasFail)
        return BTStatus::Failure;

    return BTStatus::Success;
}

void ParallelNode::Free()
{
    CompositeNode::Free();
}
