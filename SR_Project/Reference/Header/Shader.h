#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL Shader :
    public Base
{
private:
    Shader();
    virtual ~Shader();

public:
    static Shader* Create();

private:
    void Free()override;

};

END