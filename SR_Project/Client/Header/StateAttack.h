#pragma once
#include "ActionNode.h"

class StateAttackNode :
    public ActionNode
{
public:
    explicit StateAttackNode();
    virtual ~StateAttackNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;
};

