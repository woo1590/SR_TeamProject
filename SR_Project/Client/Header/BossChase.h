#pragma once
#include "ActionNode.h"
class BossChase :
    public ActionNode
{
public:
    explicit BossChase();
    virtual ~BossChase();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;
};

