#include "EnginePCH.h"
#include "CameraComponent.h"
#include "MyMath.h"
#include "EngineCore.h"
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

_matrix CameraComponent::GetViewMatrix() const
{   
    _vec3 eye = Target->GetPosition();
    _vec3 at = eye + Target->GetFoward();
    _vec3 up = Target->GetUp();

    _matrix view = math::LookAtLH(eye, at, up);

    return view;
}

_matrix CameraComponent::GetProjMatrix() const
{
    _matrix proj = math::PerspectiveFovLH(FOV, Aspect, MinZ, MaxZ);
    //D3DXMatrixPerspectiveFovLH(&proj,FOV, Aspect, MinZ, MaxZ);
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

void CameraComponent::Free()
{
}
