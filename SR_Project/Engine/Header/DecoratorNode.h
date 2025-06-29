#pragma once
#include "BTNode.h"

BEGIN(Engine)

class ENGINE_DLL DecoratorNode :
    public BTNode
{
protected:
    explicit DecoratorNode(BTNode* child = nullptr);
    virtual ~DecoratorNode();

public:
    virtual BTStatus Initialize(float dt) = 0;
    virtual BTStatus Tick(float dt, BlackBoard* bb) = 0;

    void SetChild(BTNode* childNode);

protected:
    void Free() override;

protected:
    BTNode* Child = nullptr;
};

END