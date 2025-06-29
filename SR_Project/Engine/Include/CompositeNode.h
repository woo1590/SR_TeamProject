#pragma once
#include "BTNode.h"
class CompositeNode :
    public BTNode
{
protected:
    explicit CompositeNode();
    virtual ~CompositeNode();

public:
    virtual BTStatus Initialize(float dt) = 0;
    virtual BTStatus Tick(float dt, BlackBoard* bb) = 0;

    virtual void AddChild(BTNode* child);

protected:
    void Free() override;

protected:
    std::vector<BTNode*>    ChildNodes;
    _uint                   CurrentIndex = 0;
};

