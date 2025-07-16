#pragma once
#include "ActionNode.h"

class ChangeStateNode :
    public ActionNode
{
public:
    explicit ChangeStateNode();
    virtual ~ChangeStateNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;
};

