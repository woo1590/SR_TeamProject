#pragma once
#include "DecoratorNode.h"

namespace Engine
{
    class BTNode;
}

class CheckStateChangeCount :
    public DecoratorNode
{
public:
    explicit CheckStateChangeCount(BTNode* child = nullptr);
    virtual ~CheckStateChangeCount();

public:
    virtual BTStatus Initialize(float dt);
    virtual BTStatus Tick(float dt, BlackBoard* bb);

protected:
    void Free() override;
};

