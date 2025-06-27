#include "EnginePCH.h"
#include "FreecamComponent.h"
#include "EngineCore.h"
#include "MyMath.h"

//system
#include "InputSystem.h"

//component
#include "TransformComponent.h"

FreecamComponent::FreecamComponent(Object* owner)
    :ObjectComponent(owner)
{

}

FreecamComponent::~FreecamComponent()
{
}

FreecamComponent* FreecamComponent::Create(Object* owner)
{
    FreecamComponent* Instance = new FreecamComponent(owner);

    if (FAILED(Instance->Ready_Component()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

void FreecamComponent::Update(_float dt)
{
    if (!Target) return;

    auto Input = EngineCore::GetInstance()->GetInputSystem();

    _vec3 direction{ 0.f,0.f,0.f };
    _vec3 eye = Target->GetPosition();
    _vec3 forward = Target->GetFoward();
    _vec3 right = Target->GetRight();

    if (Input->IsKeyDown(W))
        direction += forward;
    if (Input->IsKeyDown(A))
        direction -= right;
    if (Input->IsKeyDown(S))
        direction -= forward;
    if (Input->IsKeyDown(D))
        direction += right;

    if(math::Length(direction)>0.9f)
    { 
        _vec3 n;
        if (direction.y < 0.f)
            n = { 0.f,-1.f,0.f };
        else
            n = { 0.f,1.f,0.f };

        _float t = D3DXVec3Dot(&direction, &n) / D3DXVec3Dot(&n, &n);
        _vec3 proj = n * t;
        direction -= proj;
    }

    if (Input->IsKeyDown(SPACE))
        direction = { 0.f,1.f,0.f };
    if (Input->IsKeyDown(LSHIFT))
        direction = { 0.f,-1.f,0.f };

    D3DXVec3Normalize(&direction, &direction);
    Target->Translate(direction * 300.f * dt);

    _vec2 offset = Input->GetMouseDelta() * Sensitivity;

    Yaw += offset.x;
    Pitch -= offset.y;
    
    Pitch = std::clamp(Pitch, -89.f, 89.f);

    _float cy = cos(D3DXToRadian(Yaw));
    _float cp = cos(D3DXToRadian(Pitch));
    _float sp = sin(D3DXToRadian(Pitch));
    _float sy = sin(D3DXToRadian(Yaw));

    _vec3 front{ sy * cp,sp,cy * cp };
    Target->SetForward(front);
}

void FreecamComponent::SetTarget(Object* target)
{

}

void FreecamComponent::SetTarget(TransformComponent* target)
{
    Target = target;
}

void FreecamComponent::SetSensitivity(_float sensitivity)
{
    Sensitivity = sensitivity;
}

void FreecamComponent::Free()
{
    ObjectComponent::Free();
}
