#pragma once
#include "DecoratorNode.h"

class SucceederDecorator :
    public DecoratorNode
{
protected:
    explicit SucceederDecorator(BTNode* child = nullptr);
    virtual ~SucceederDecorator();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;
};

