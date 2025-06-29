#pragma once
#include "CompositeNode.h"

class SequenceNode :
    public CompositeNode
{
protected:
    explicit SequenceNode();
    virtual ~SequenceNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt) override;

protected:
    void Free() override;

};

