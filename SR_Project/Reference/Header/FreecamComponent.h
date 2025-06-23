#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class TransformComponent;
class ENGINE_DLL FreecamComponent :
    public ObjectComponent
{
private:
    FreecamComponent(Object* owner);
    virtual ~FreecamComponent();

public:
    static FreecamComponent* Create(Object* owner);
    void Update(_float dt)override;

    void SetTarget(Object* target);
    void SetTarget(TransformComponent* target);
    void SetSensitivity(_float sensitivity);
private:
    void Free()override;

    TransformComponent* Target = nullptr;

    _float Sensitivity = 0.1f;
    _float Pitch = 0.f;
    _float Yaw = 0.f; 
    _float Roll = 0.f;

    _vec2 CenterPos{ WINCX * 0.5f,WINCY * 0.5f };
};

END