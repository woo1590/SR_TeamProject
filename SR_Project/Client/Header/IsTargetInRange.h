#pragma once
#include "DecoratorNode.h"

namespace Engine
{
    class BTNode;
}

class IsTargetInRange :
    public DecoratorNode
{
public:
    explicit IsTargetInRange(BTNode* child = nullptr);
    virtual ~IsTargetInRange();

public:
    virtual BTStatus Initialize(float dt);
    virtual BTStatus Tick(float dt, BlackBoard* bb);

protected:
    void Free() override;

};

