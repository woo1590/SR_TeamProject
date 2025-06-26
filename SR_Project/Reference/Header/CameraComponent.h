#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class TransformComponent;
class ENGINE_DLL CameraComponent : public ObjectComponent
{
public:
    enum class CamType { Static, Target, Free, Count };
private:
    explicit CameraComponent(Object* owner);
    virtual ~CameraComponent();

public:
    static CameraComponent* Create(Object* owner);

    void SetFOV(float fov);

    void SetAspect(float width, float height);
    void SetAspect(float aspect);

    void SetMinMaxZ(float minZ, float maxZ);

    void SetTarget(Object* target);
    void SetTarget(TransformComponent* target);

    _matrix GetViewMatrix()const;
    _matrix GetProjMatrix()const;
private:
    void Free()override;

    _float FOV = D3DXToRadian(60.f);
    _float Aspect = WINCX / WINCY;
    _float MinZ = 0.1f;
    _float MaxZ = 1000.f;

    TransformComponent* Target = nullptr;
};

END