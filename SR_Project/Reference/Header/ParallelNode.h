#pragma once
#include "CompositeNode.h"

BEGIN(Engine)

class ENGINE_DLL ParallelNode :
    public CompositeNode
{
protected:
    explicit ParallelNode();
    virtual ~ParallelNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;

};

END