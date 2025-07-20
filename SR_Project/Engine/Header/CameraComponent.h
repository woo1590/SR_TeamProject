#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class TransformComponent;
class ENGINE_DLL CameraComponent : public ObjectComponent
{
public:
    enum class CamType { Static, Target, Free, Count };
    enum class ProjectionType {Perspective, Orthographic};
private:
    explicit CameraComponent(Object* owner);
    virtual ~CameraComponent();

public:
    static CameraComponent* Create(Object* owner);
    HRESULT Ready_Component()override;

    void SetFOV(float fov);

    void SetAspect(float width, float height);
    void SetAspect(float aspect);
    void SetMinMaxZ(float minZ, float maxZ);

    // -------------------------
    void SetProjectionType(ProjectionType type) { projType = type; }
    void SetOrthoSize(float width, float height);

    // --------------------------

    _matrix GetViewMatrix()const;
    _matrix GetProjMatrix()const;
    Ray ScreenPointRay();

    TransformComponent* GetTarget() { return Target; };
private:
    void Free()override;

    _float FOV = D3DXToRadian(60.f);
    _float Aspect = WINCX / WINCY;
    _float MinZ = 0.1f;
    _float MaxZ = 1000.f;

    _vec3 shakeOffset{ 0.f,0.f,0.f };

    TransformComponent* Target = nullptr;

    // --------------------------------
    ProjectionType projType = ProjectionType::Perspective;
    float orthoWidth = WINCX;
    float orthoHeight = WINCY;
};

END