#include "EnginePCH.h"
#include "ThirdcamComponent.h"
#include "EngineCore.h"
#include "Random.h"

//object
#include "Object.h"

//component
#include "TransformComponent.h"

ThirdcamComponent::ThirdcamComponent(Object* owner)
    :ObjectComponent(owner)
{
}

ThirdcamComponent::~ThirdcamComponent()
{
}

ThirdcamComponent* ThirdcamComponent::Create(Object* owner)
{
    ThirdcamComponent* Instance = new ThirdcamComponent(owner);

    if (FAILED(Instance->Ready_Component()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT ThirdcamComponent::Ready_Component()
{
    Target = owner->GetComponent<TransformComponent>();
    if (!Target)
        return E_FAIL;

    D3DXVec3Normalize(&Direction, &Offset);
    Direction *= -1.f;

    return S_OK;
}

void ThirdcamComponent::Update(_float dt)
{
    if (!FollowTarget)
        return;

    ObjectComponent::Update(dt);

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

        shakeOffset.x = r->get<_float>(-power, power) * progress * progress * 0.5f;
        shakeOffset.z = r->get<_float>(-power, power) * progress * progress * 0.5f;

        shakeTimer += dt;
    }

    _vec3 targetPos = FollowTarget->GetPosition();
    _vec3 camPos = targetPos + Offset + shakeOffset;

    Target->SetPosition(camPos);
    Target->SetForward(Direction);
}

void ThirdcamComponent::SetFollowTarget(TransformComponent* target)
{
    FollowTarget = target;
}

void ThirdcamComponent::SetFollowTarget(Object* target)
{
    FollowTarget = target->GetComponent<TransformComponent>();
}

void ThirdcamComponent::SetOffset(_vec3 offset)
{
    Offset = offset;    
    D3DXVec3Normalize(&Direction, &Offset);
    Direction *= -1.f;
}

void ThirdcamComponent::SetOffset(_float x, _float y, _float z)
{
    SetOffset(_vec3(x, y, z));
}

void ThirdcamComponent::SetShake(_float power, _float duration)
{
    this->power = power;
    shakeDuration = duration;
}

void ThirdcamComponent::Free()
{
    ObjectComponent::Free();
}
