#pragma once
#include "Base.h"

enum BTStatus
{
    Success,
    Failure,
    Running,
    End
};

class BTNode :
    public Base
{
protected:
    explicit BTNode();
    virtual ~BTNode();

public:
    virtual BTStatus Initialize(float dt) = 0;
    virtual BTStatus Tick(float dt) = 0;

protected:
    void Free()override;
};

