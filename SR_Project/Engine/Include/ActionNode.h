#pragma once
#include "BTNode.h"

class ActionNode :
    public BTNode
{
protected:
    explicit ActionNode();
    virtual ~ActionNode();

public:
    virtual BTStatus Initialize(float dt) = 0;
    virtual BTStatus Tick(float dt, BlackBoard* bb) = 0;

protected:
    void Free() override;
};

