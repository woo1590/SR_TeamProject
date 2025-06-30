#pragma once
#include "CompositeNode.h"

BEGIN(Engine)

class ENGINE_DLL RandomSelectorNode :
    public CompositeNode
{
protected:
    explicit RandomSelectorNode();
    virtual ~RandomSelectorNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;

protected:
    std::vector<_uint> RandomIndexes;
};

END