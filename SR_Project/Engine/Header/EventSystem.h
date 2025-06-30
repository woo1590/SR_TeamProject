#pragma once
#include "Base.h"

BEGIN(Engine)

class CameraComponent;
class ENGINE_DLL EventSystem :
    public Base
{
private:
    EventSystem();
    virtual ~EventSystem();

public:
    static EventSystem* Create();
    HRESULT Ready_EventSystem();
    void Update();

private:
    void Free()override;

    CameraComponent* Camera = nullptr;
};

END