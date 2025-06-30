#pragma once
#include "CompositeNode.h"

BEGIN(Engine)

class ENGINE_DLL SelectorNode :
    public CompositeNode
{
public:
    explicit SelectorNode();
    virtual ~SelectorNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;
};

END