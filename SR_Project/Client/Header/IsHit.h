#pragma once
#include "DecoratorNode.h"

namespace Engine
{
    class BTNode;
}

class IsHitNode :
    public DecoratorNode
{
public:
    explicit IsHitNode(BTNode* child = nullptr);
    virtual ~IsHitNode();

public:
    virtual BTStatus Initialize(float dt);
    virtual BTStatus Tick(float dt, BlackBoard* bb);

protected:
    void Free() override;
};

