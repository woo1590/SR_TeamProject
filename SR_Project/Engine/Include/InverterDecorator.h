#pragma once
#include "DecoratorNode.h"
class InverterDecorator :
    public DecoratorNode
{
protected:
    explicit InverterDecorator(BTNode* child = nullptr);
    virtual ~InverterDecorator();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;

};
