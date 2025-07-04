#pragma once
#include "ActionNode.h"


class HitNode :
    public ActionNode
{
public:
    explicit HitNode();
    virtual ~HitNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;

private:
    int prevHp;
};

