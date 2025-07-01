#pragma once
#include "DecoratorNode.h"

namespace Engine
{
    class BTNode;
}

class IsAliveNode :
    public DecoratorNode
{
public:
    explicit IsAliveNode(BTNode* child = nullptr);
    virtual ~IsAliveNode();

public:
    virtual BTStatus Initialize(float dt);
    virtual BTStatus Tick(float dt, BlackBoard* bb);

protected:
    void Free() override;
};

