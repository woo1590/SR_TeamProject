#pragma once
#include "DecoratorNode.h"
class RepeatDecorator :
    public DecoratorNode
{
protected:
    explicit RepeatDecorator(BTNode* child = nullptr);
    virtual ~RepeatDecorator();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;

protected:
    _uint       RepeatCnt;
    _uint       CurrentCnt;
};

