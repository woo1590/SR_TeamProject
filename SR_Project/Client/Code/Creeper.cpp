#include "pch.h"
#include "Creeper.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"

Creeper::Creeper(ObjectManager* owner, ObjectType objType)
    :Monster(owner, objType)
{
}

Creeper::~Creeper()
{
}

Creeper* Creeper::Create(ObjectManager* owner, ObjectType objType)
{
    Creeper* Instance = new Creeper(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Creeper::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Monster::Ready_Object(owner, objType);

    //InitTransform
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);
    InitTransform(objType);

    //Init Collision
    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(2.f, 5.5f, 2.f));

    //Create BT
    //InitTree();

    //Animation
    InitAnimation();
    return S_OK;
}

void Creeper::Update(_float dt)
{
    Monster::Update(dt);
    PlayWalk(dt);
}

void Creeper::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void Creeper::MoveTo(_vec3* dir, _float dt)
{
}

void Creeper::RotateTo(_vec3* dir, float dt)
{
}

void Creeper::Attack(Object* target)
{
}

void Creeper::Die()
{
}

void Creeper::InitTransform(ObjectType objType)
{
    SetMaterial(L"CreeperFace_Mtrl", "Head");
    SetMaterial(L"CreeperBody_Mtrl", "Body");
    SetMaterial(L"CreeperLeg_Mtrl", "LArm");
    SetMaterial(L"CreeperLeg_Mtrl", "RArm");
    SetMaterial(L"CreeperLeg_Mtrl", "LLeg");
    SetMaterial(L"CreeperLeg_Mtrl", "RLeg");

    //body
    SetScale(_vec3(7.f * Scale, 16.f * Scale, 4.f * Scale), "Body");

    //arm
    SetScale(_vec3(5.f * Scale, 5.f * Scale, 5.f * Scale), "LArm");               //front
    SetPosition(_vec3(-6.f * Scale, -20.f * Scale, 5.5f * Scale), "LArm");       //front
    Bones["LArm"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 10.f * Scale, 0.0f));
    SetScale(_vec3(5.f * Scale, 5.f * Scale, 5.f * Scale), "RArm");               //front
    SetPosition(_vec3(6.f * Scale, -20.f * Scale, 5.5f * Scale), "RArm");        //front
    Bones["RArm"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 10.f * Scale, 0.0f));

    //leg
    SetScale(_vec3(5.f * Scale, 5.f * Scale, 5.f * Scale), "LLeg");               //back
    SetPosition(_vec3(-6.f * Scale, -20.f * Scale, -5.5f * Scale), "LLeg");      //back
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 10.f * Scale, 0.0f));
    SetScale(_vec3(5.f * Scale, 5.f * Scale, 5.f * Scale), "RLeg");               //back
    SetPosition(_vec3(6.f * Scale, -20.f * Scale, -5.5f * Scale), "RLeg");       //back
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 10.f * Scale, 0.0f));
}

void Creeper::InitTree()
{
}

void Creeper::InitAnimation()
{
    WalkAnim.ElapsedTime = 0.f;
}

void Creeper::PlayAnimation(_float dt)
{
}

void Creeper::PlayIdle(_float dt)
{
}

void Creeper::PlayWalk(_float dt)
{
    WalkAnim.ElapsedTime += dt;

    float Angle = sinf(WalkAnim.ElapsedTime * 7.f);
    SetRotation({ Angle/ 2, 0.f, 0.f }, "LLeg");
    SetRotation({ -Angle / 2, 0.f, 0.f }, "RLeg");

    SetRotation({ Angle / 2, 0.f, 0.f }, "LArm");
    SetRotation({ -Angle / 2, 0.f, 0.f }, "RArm");
}

void Creeper::PlayAttack(_float dt)
{
}

void Creeper::PlayDie(_float dt)
{
}

void Creeper::OnCollisionStay(Object* other)
{
}

void Creeper::Free()
{
    Monster::Free();
}
