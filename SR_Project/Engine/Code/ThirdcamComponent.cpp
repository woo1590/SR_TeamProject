#include "EnginePCH.h"
#include "ThirdcamComponent.h"

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

    _vec3 targetPos = FollowTarget->GetPosition();
    _vec3 camPos = targetPos + Offset;


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

void ThirdcamComponent::Free()
{
    ObjectComponent::Free();
}
