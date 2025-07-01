#pragma once
#include "ActionNode.h"
class ChaseNode :
    public ActionNode
{
public:
    explicit ChaseNode();
    virtual ~ChaseNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;
};

