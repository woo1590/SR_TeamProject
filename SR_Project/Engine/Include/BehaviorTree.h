#pragma once
#include "Base.h"
#include "BTNode.h"
#include "BlackBoard.h"

class BehaviorTree :
    public Base
{
protected:
    explicit BehaviorTree(BTNode* rootNode = nullptr);
    virtual ~BehaviorTree();

public:
    static BehaviorTree* Create(BTNode* rootNode);

    void Tick(float dt, BlackBoard* bb);        

    void SetRoot(BTNode* rootNode);

protected:
    void Free() override;

protected:
    BTNode* RootNode = nullptr;
};

