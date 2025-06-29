#pragma once
#include "CompositeNode.h"

class SelectorNode :
    public CompositeNode
{
protected:
    explicit SelectorNode();
    virtual ~SelectorNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt) override;

protected:
    void Free() override;
};

