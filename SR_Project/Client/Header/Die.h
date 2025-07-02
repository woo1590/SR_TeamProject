#pragma once
#include "ActionNode.h"
class DieNode :
    public ActionNode
{
public:
    explicit DieNode();
    virtual ~DieNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;
};

