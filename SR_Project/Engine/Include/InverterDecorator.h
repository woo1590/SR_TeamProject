#pragma once
#include "DecoratorNode.h"
class InverterDecorator :
    public DecoratorNode
{
public:
    explicit InverterDecorator(BTNode* child = nullptr);
    virtual ~InverterDecorator();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt) override;

protected:
    void Free() override;

};
