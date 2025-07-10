#pragma once
#include "ActionNode.h"
class BackStepNode :
    public ActionNode
{
public:
    explicit BackStepNode();
    virtual ~BackStepNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;
};

