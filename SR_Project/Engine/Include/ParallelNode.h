#pragma once
#include "CompositeNode.h"

class ParallelNode :
    public CompositeNode
{
protected:
    explicit ParallelNode();
    virtual ~ParallelNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt) override;

protected:
    void Free() override;

};

