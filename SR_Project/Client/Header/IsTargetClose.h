#pragma once
#include "DecoratorNode.h"

namespace Engine
{
    class BTNode;
}

class IsTargetClose :
    public DecoratorNode
{
public:
    explicit IsTargetClose(BTNode* child = nullptr);
    virtual ~IsTargetClose();

public:
    virtual BTStatus Initialize(float dt);
    virtual BTStatus Tick(float dt, BlackBoard* bb);

protected:
    void Free() override;
};

