#pragma once
#include "DecoratorNode.h"

namespace Engine
{
    class BTNode;
}

class IsTargetInAttackRange :
    public DecoratorNode
{
public:
    explicit IsTargetInAttackRange(BTNode* child = nullptr);
    virtual ~IsTargetInAttackRange();

public:
    virtual BTStatus Initialize(float dt);
    virtual BTStatus Tick(float dt, BlackBoard* bb);

protected:
    void Free() override;
};

