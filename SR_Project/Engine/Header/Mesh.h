#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL Mesh :
    public Base
{
protected:
    Mesh();
    virtual ~Mesh();

public:
    virtual void Draw()PURE;

protected:
    void Free();

    LPDIRECT3DDEVICE9 Device = nullptr;
};

END