#pragma once
#include "DecoratorNode.h"

namespace Engine
{
    class BTNode;
}

class IsRandomMoveCheck :
    public DecoratorNode
{
public:
    explicit IsRandomMoveCheck(BTNode* child = nullptr);
    virtual ~IsRandomMoveCheck();

public:
    virtual BTStatus Initialize(float dt);
    virtual BTStatus Tick(float dt, BlackBoard* bb);

protected:
    void Free() override;
};

