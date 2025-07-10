#include "EnginePCH.h"
#include "ToolcamComponent.h"
#include "EngineCore.h"
#include "MyMath.h"

//system
#include "InputSystem.h"
#include "TimerManager.h"

//object
#include "Object.h"

//component
#include "TransformComponent.h"

ToolcamComponent::ToolcamComponent(Object* owner) : ObjectComponent(owner)
{
}

ToolcamComponent::~ToolcamComponent()
{
}

ToolcamComponent* ToolcamComponent::Create(Object* owner)
{
    ToolcamComponent* Instance = new ToolcamComponent(owner);

    if (FAILED(Instance->Ready_Component()))
    {
        Safe_Release(Instance);
        MessageBoxA(0, "ToolCam Created Failed", "Error", 0);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT ToolcamComponent::Ready_Component()
{
    Target = owner->GetComponent<TransformComponent>();
    if (!Target)
        return E_FAIL;

    return S_OK;
}

void ToolcamComponent::Update(_float dt)
{
    if (!Target) return;

    auto Input = EngineCore::GetInstance()->GetInputSystem();

    if (Input->IsKeyPressed(TAB)) Fixed = !Fixed;

    _vec3 direction{ 0.f, 0.f, 0.f };
    _vec3 forward = Target->GetFoward();
    _vec3 right = Target->GetRight();

    if (Input->IsKeyDown(W)) direction += forward;
    if (Input->IsKeyDown(A)) direction -= right;
    if (Input->IsKeyDown(S)) direction -= forward;
    if (Input->IsKeyDown(D)) direction += right;

    if (math::Length(direction) > 0.9f)
    {
        _vec3 n = direction.y < 0.f ? _vec3{ 0.f, -1.f, 0.f } : _vec3{ 0.f, 1.f, 0.f };
        _float t = D3DXVec3Dot(&direction, &n) / D3DXVec3Dot(&n, &n);
        direction -= n * t;
    }

    if (Input->IsKeyDown(SPACE)) direction = { 0.f, 1.f, 0.f };
    if (Input->IsKeyDown(LSHIFT)) direction = { 0.f, -1.f, 0.f };

    D3DXVec3Normalize(&direction, &direction);
    Target->Translate(direction * Speed * dt);

    if (Fixed) return;

    _vec2 offset = Input->GetMouseDelta() * Sensitivity;

    Yaw += offset.x;
    Pitch -= offset.y;
    Pitch = std::clamp(Pitch, -89.f, 89.f);

    _float cy = cos(D3DXToRadian(Yaw));
    _float cp = cos(D3DXToRadian(Pitch));
    _float sp = sin(D3DXToRadian(Pitch));
    _float sy = sin(D3DXToRadian(Yaw));

    _vec3 front{ sy * cp, sp, cy * cp };
    Target->SetForward(front);
}

void ToolcamComponent::Free()
{
    ObjectComponent::Free();
}