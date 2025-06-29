#pragma once
#include "Base.h"
#include "BlackBoard.h"

enum BTStatus
{
    Success,
    Failure,
    Running,
    End
};

BEGIN(Engine)

class ENGINE_DLL BTNode :
    public Base
{
protected:
    explicit BTNode();
    virtual ~BTNode();

public:
    virtual BTStatus Initialize(float dt) = 0;
    virtual BTStatus Tick(float dt, BlackBoard* bb) = 0;

protected:
    void Free()override;
};

END