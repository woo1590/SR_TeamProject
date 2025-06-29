#pragma once
#include "CompositeNode.h"

BEGIN(Engine)

class ENGINE_DLL SequenceNode :
    public CompositeNode
{
protected:
    explicit SequenceNode();
    virtual ~SequenceNode();

public:
    BTStatus Initialize(float dt) override;
    BTStatus Tick(float dt, BlackBoard* bb) override;

protected:
    void Free() override;

};

END