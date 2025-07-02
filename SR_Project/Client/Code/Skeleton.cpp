#include "pch.h"
#include "Skeleton.h"
#include "TransformComponent.h"

Skeleton::Skeleton(ObjectManager* owner, ObjectType objType)
	:Monster(owner, objType)
{
}

Skeleton::~Skeleton()
{
}

Skeleton* Skeleton::Create(ObjectManager* owner, ObjectType objType)
{
    Skeleton* Instance = new Skeleton(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Skeleton::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    BaseCharacter::Ready_Object(owner, objType);

    SetMaterial(L"SkeletonBody_Mtrl", "Body", RENDER_ID::Render_Alpha);
    SetMaterial(L"SkeletonFace_Mtrl", "Head", RENDER_ID::Render_Alpha);
    SetMaterial(L"SkeletonBone_Mtrl", "LHand");
    SetMaterial(L"SkeletonBone_Mtrl", "RHand");
    SetMaterial(L"SkeletonBone_Mtrl", "LLeg");
    SetMaterial(L"SkeletonBone_Mtrl", "RLeg");

    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);
    transform->SetPosition(_vec3(-20.f, 0.f, 0.f));

    //head
    SetScale(_vec3(7.f * Scale, 7.f * Scale, 7.f * Scale), "Head");
    SetPosition(_vec3(0.f, 17.5f * Scale, 0.f), "Head");
    //body
    SetScale(_vec3(8.f * Scale, 12.f * Scale, 2.f * Scale), "Body");
    //hand
    SetScale(_vec3(2.f * Scale, 10.f * Scale, 2.f * Scale), "LHand");
    SetPosition(_vec3(-9.f * Scale, 0.f, 0.f), "LHand");
    SetScale(_vec3(2.f * Scale, 10.f * Scale, 2.f * Scale), "RHand");
    SetPosition(_vec3(9.f * Scale, 0.f, 0.f), "RHand");
    //leg
    SetScale(_vec3(2.f * Scale, 12.f * Scale, 2.f * Scale), "LLeg");
    SetScale(_vec3(2.f * Scale, 12.f * Scale, 2.f * Scale), "RLeg");
	return S_OK;
}

void Skeleton::Update(_float dt)
{
    WalkAnim.ElapsedTime += dt;

    float Angle = sinf(WalkAnim.ElapsedTime * 7.f);
    SetRotation({ Angle, 0.f, 0.f }, "LLeg");
    SetRotation({ -Angle, 0.f, 0.f }, "RLeg");

    SetRotation({ -Angle, 0.f, 0.f }, "LHand");
    SetRotation({ Angle, 0.f, 0.f }, "RHand");
}

void Skeleton::Late_Update(_float dt)
{
}

void Skeleton::Free()
{
    BaseCharacter::Free();
}
