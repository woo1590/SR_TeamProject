#pragma once
#include "DecoratorNode.h"
class RepeatDecorator :
    public DecoratorNode
{
public:
    explicit RepeatDecorator(BTNode* child = nullptr);
    virtual ~RepeatDecorator();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt) override;

protected:
    void Free() override;

protected:
    _uint       RepeatCnt;
    _uint       CurrentCnt;
};

