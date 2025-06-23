#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL Frame :
    public Base
{
private:
    Frame();
    virtual~Frame();

public:
    HRESULT Ready_Frame(const _float callLimit);
    _bool IsPermitCall(_float dt);

public:
    static Frame* Create(const _float callLimit);

private:
    _float CallLimit;
    _float AccDeltaTime;

    void Free()override;
};

END