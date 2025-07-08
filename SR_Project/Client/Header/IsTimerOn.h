#pragma once
#include "DecoratorNode.h"

namespace Engine
{
    class BTNode;
}

class IsTimerOn :
    public DecoratorNode
{
public:
    explicit IsTimerOn(BTNode* child = nullptr);
    virtual ~IsTimerOn();

public:
    virtual BTStatus Initialize(float dt);
    virtual BTStatus Tick(float dt, BlackBoard* bb);

protected:
    void Free() override;
};

