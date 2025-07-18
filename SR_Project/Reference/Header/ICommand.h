#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL ICommand :
    public Base
{
protected:
    ICommand() = default;
    virtual ~ICommand() = default;

public:
    virtual void Execute()PURE;

protected:
    void Free()override{}
};

END