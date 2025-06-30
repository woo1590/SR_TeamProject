#include "pch.h"
#include "Camera.h"
#include "EngineCore.h"
#include "RenderSystem.h"

//component
#include "FreecamComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

Camera::Camera(ObjectManager* owner, ObjectType objType) : Object(owner, objType)
{
}

Camera::~Camera()
{
}

Camera* Camera::Create(ObjectManager* owner, ObjectType objType)
{
    Camera* Instance = new Camera(owner, objType);

    if (FAILED(Instance->Ready_Object()))
    {
        Safe_Release(Instance);
        MSG_BOX("Camera Create Failed");
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Camera::Ready_Object()
{
    Object::Ready_Object();

    auto transform = AddComponent<TransformComponent>();

    auto fCam = AddComponent<FreecamComponent>();
    fCam->SetTarget(transform);
    fCam->SetSensitivity(0.5f);

    auto cam = AddComponent<CameraComponent>();
    cam->SetTarget(transform);

    EngineCore::GetInstance()->GetRenderSystem()->SetCamera(cam);

    _vec3 Pos{ -10.f, 10.f, -10.f };
    transform->SetPosition(Pos);

    return S_OK;
}

void Camera::Update(_float dt)
{
    Object::Update(dt);
}

void Camera::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Camera::Free()
{
    Object::Free();
}