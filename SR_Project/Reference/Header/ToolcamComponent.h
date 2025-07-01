#pragma once
#include "EnginePCH.h"
#include "ObjectComponent.h"

BEGIN(Engine)
class TransformComponent;

class ENGINE_DLL ToolcamComponent : public ObjectComponent
{
private:
    ToolcamComponent(Object* owner);
    virtual ~ToolcamComponent();

public:
    static ToolcamComponent* Create(Object* owner);
    HRESULT Ready_Component()override;
    void Update(_float dt)override;

    void SetSpeed(_float speed) { Speed = speed; }
    void SetSensitivity(_float sensitivity) { Sensitivity = sensitivity; }

    _bool GetFixed() { return Fixed; }

private:
    void Free()override;

private:
    _bool Fixed = false;

    _float Yaw = 0.f;
    _float Roll = 0.f;
    _float Pitch = 0.f;
    _float Speed = 20.f;
    _float Sensitivity = 0.65f;

    _vec2 CenterPos{ WINCX * 0.5f, WINCY * 0.5f };

    TransformComponent* Target = nullptr;
};
END