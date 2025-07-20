#pragma once
#include "ActionNode.h"

class TeleportNode :
    public ActionNode
{
public:
    explicit TeleportNode();
    virtual ~TeleportNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;

};

