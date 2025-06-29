#include "EnginePCH.h"
#include "RandomSelectorNode.h"

RandomSelectorNode::RandomSelectorNode()
{
}

RandomSelectorNode::~RandomSelectorNode()
{
}

BTStatus RandomSelectorNode::Initialize(float dt)
{
    CurrentIndex = 0;

    RandomIndexes.clear();
    for(_uint i = 0; i < ChildNodes.size(); ++i)
        RandomIndexes.push_back(i);

    std::shuffle(RandomIndexes.begin(), RandomIndexes.end(), std::mt19937{ std::random_device{}() });

    for (auto& child : ChildNodes) 
    {
        if (child)
            child->Initialize(dt);
    }

    return BTStatus::Running;
}

BTStatus RandomSelectorNode::Tick(float dt)
{
    if (CurrentIndex >= RandomIndexes.size() || 
        RandomIndexes[CurrentIndex] >= ChildNodes.size())
        return BTStatus::Failure;

    BTStatus status = ChildNodes[RandomIndexes[CurrentIndex]]->Tick(dt);

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

void RandomSelectorNode::Free()
{
    CompositeNode::Free();
}
