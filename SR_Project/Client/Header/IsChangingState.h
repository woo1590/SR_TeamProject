#pragma once
#include "DecoratorNode.h"

namespace Engine
{
    class BTNode;
}

class IsChangingState :
    public DecoratorNode
{
public:
    explicit IsChangingState(BTNode* child = nullptr);
    virtual ~IsChangingState();

public:
    virtual BTStatus Initialize(float dt);
    virtual BTStatus Tick(float dt, BlackBoard* bb);

protected:
    void Free() override;
};

