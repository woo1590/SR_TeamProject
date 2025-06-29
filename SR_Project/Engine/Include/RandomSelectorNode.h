#pragma once
#include "CompositeNode.h"

class RandomSelectorNode :
    public CompositeNode
{
protected:
    explicit RandomSelectorNode();
    virtual ~RandomSelectorNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt) override;

protected:
    void Free() override;

protected:
    std::vector<_uint> RandomIndexes;
};
