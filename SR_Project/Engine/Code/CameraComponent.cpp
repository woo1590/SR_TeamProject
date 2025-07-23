#include "EnginePCH.h"
#include "CameraComponent.h"
#include "MyMath.h"
#include "EngineCore.h"
#include "Random.h"
#include "InputSystem.h"
#include "GraphicDevice.h"

//object
#include "Object.h"

//component
#include "TransformComponent.h"

CameraComponent::CameraComponent(Object* owner)
    :ObjectComponent(owner)
{
}

CameraComponent::~CameraComponent()
{
}

CameraComponent* CameraComponent::Create(Object* owner)
{   
    CameraComponent* Instance = new CameraComponent(owner);

    if (FAILED(Instance->Ready_Component()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT CameraComponent::Ready_Component()
{
    Target = owner->GetComponent<TransformComponent>();
    if (!Target)
        return E_FAIL;

    return S_OK;
}

void CameraComponent::Update(_float dt)
{
    if (shakeTimer >= shakeDuration)
    {
        shakeDuration = 0.f;
        shakeTimer = 0.f;
        shakeOffset = { 0.f,0.f,0.f };
    }

    if (shakeDuration > 0.f)
    {
        auto r = EngineCore::GetInstance()->GetRandom();

        _float progress = 1.f - (shakeTimer / shakeDuration);
        if (progress < 0.f)
            progress = 0.f;

        shakeOffset.x = r->get<_float>(-power, power) * progress * progress * 0.1f;
        shakeOffset.z = r->get<_float>(-power, power) * progress * progress * 0.1f;

        shakeTimer += dt;
    }

    _vec3 targetPos = Target->GetPosition();          //목표 대상 위치 = 플레이어 위치
    _vec3 finalCamPos = targetPos + shakeOffset;   //카메라 최종위치 = 플레이어 위치 + offset

    Target->SetPosition(finalCamPos);
}

void CameraComponent::SetFOV(float fov)
{
    FOV = fov;
}

void CameraComponent::SetAspect(float width, float height)
{
    Aspect = width / height;
}

void CameraComponent::SetAspect(float aspect)
{
    Aspect = aspect;
}

void CameraComponent::SetMinMaxZ(float minZ, float maxZ)
{
    MinZ = minZ;
    MaxZ = maxZ;
}

void CameraComponent::SetOrthoSize(float width, float height)
{
    orthoWidth = width;
    orthoHeight = height;
}

_matrix CameraComponent::GetViewMatrix() const
{   
    _vec3 eye = Target->GetPosition();
    _vec3 at = eye + Target->GetFoward();
    _vec3 up = Target->GetUp();

    _matrix view;
    D3DXMatrixLookAtLH(&view, &eye, &at, &up);

    return view;
}

_matrix CameraComponent::GetProjMatrix() const
{
    _matrix proj;
    
    switch (projType)
    {
    case ProjectionType::Perspective:
        D3DXMatrixPerspectiveFovLH(&proj, FOV, Aspect, MinZ, MaxZ);
        break;
    case ProjectionType::Orthographic:
        D3DXMatrixOrthoLH(&proj, orthoWidth, orthoHeight, MinZ, MaxZ);
        break;
    }
    return proj;
}

Ray CameraComponent::ScreenPointRay()
{
    _vec3 mousePos = EngineCore::GetInstance()->GetInputSystem()->GetMousePos();
    Ray ray;

    D3DVIEWPORT9 vp;
    GraphicDevice::GetInstance()->GetDevice()->GetViewport(&vp);
    
    _matrix view = GetViewMatrix();
    D3DXMatrixInverse(&view, nullptr, &view);

    _matrix proj = GetProjMatrix();
    D3DXMatrixInverse(&proj, nullptr, &proj);

    mousePos.x = (((2.f * mousePos.x) / vp.Width) - 1.f);
    mousePos.y = (((-2.f * mousePos.y) / vp.Height) + 1.f);
    D3DXVec3TransformCoord(&mousePos, &mousePos, &proj);

    ray.Origin = { 0.f,0.f,0.f };
    ray.Direction = mousePos - ray.Origin;

    D3DXVec3TransformCoord(&ray.Origin, &ray.Origin, &view);
    D3DXVec3TransformNormal(&ray.Direction, &ray.Direction, &view);

    return ray;
}

void CameraComponent::SetShake(_float power, _float duration)
{
    this->power = power;
    shakeDuration = duration;
}

void CameraComponent::Free()
{
}
