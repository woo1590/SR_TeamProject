#pragma once
#include "ActionNode.h"
class Chase :
    public ActionNode
{
public:
    explicit Chase();
    virtual ~Chase();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;
};

