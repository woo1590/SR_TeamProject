#pragma once
#include "DecoratorNode.h"

namespace Engine
{
    class BTNode;
}

class IsTargetInDetectRange :
    public DecoratorNode
{
protected:
    explicit IsTargetInDetectRange(BTNode* child = nullptr);
    virtual ~IsTargetInDetectRange();

public:
    virtual BTStatus Initialize(float dt);
    virtual BTStatus Tick(float dt, BlackBoard* bb);

protected:
    void Free() override;
};

