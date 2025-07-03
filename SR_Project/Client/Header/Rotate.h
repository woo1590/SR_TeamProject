#pragma once
#include "ActionNode.h"

class RotateNode :
    public ActionNode
{
public:
    explicit RotateNode();
    virtual ~RotateNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;
};

