#pragma once
#include "Base.h"

BEGIN(Engine)

class CameraComponent;
class Scene;
class ENGINE_DLL EventSystem :
    public Base
{
private:
    EventSystem(Scene* owner);
    virtual ~EventSystem();

public:
    static EventSystem* Create(Scene* owner);
    HRESULT Ready_EventSystem();
    void Update();

    void SetCamera();
private:
    void Free()override;

    CameraComponent* Camera = nullptr;
    Scene* owner = nullptr;
};

END