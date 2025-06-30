#pragma once
#include "ActionNode.h"

class AttackNode :
    public ActionNode
{
public:
    explicit AttackNode();
    virtual ~AttackNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;
};

