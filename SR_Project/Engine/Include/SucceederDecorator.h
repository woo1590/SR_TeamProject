#pragma once
#include "DecoratorNode.h"

class SucceederDecorator :
    public DecoratorNode
{
public:
    explicit SucceederDecorator(BTNode* child = nullptr);
    virtual ~SucceederDecorator();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt) override;

protected:
    void Free() override;
};

