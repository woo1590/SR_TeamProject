#include "pch.h"
#include "Slime.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

Slime::Slime(ObjectManager* owner, ObjectType objType)
	:Monster(owner, objType)
{
}

Slime::~Slime()
{
}

Slime* Slime::Create(ObjectManager* owner, ObjectType objType)
{
    Slime* Instance = new Slime(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Slime::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Monster::Ready_Object(owner, objType);

    InitTransform(objType);

    //Init Collision
    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(10.f, 10.f, 10.f));

    //Create BT
    //InitTree();

    //Animation
    InitAnimation();
    return S_OK;
}

void Slime::Update(_float dt)
{
    Monster::Update(dt);
}

void Slime::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void Slime::MoveTo(_vec3* dir, _float dt)
{
}

void Slime::RotateTo(_vec3* dir, float dt)
{
}

void Slime::Attack(Object* target)
{
}

void Slime::Die()
{
}

void Slime::Hit(_vec3 dir, _float power)
{
}

void Slime::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    transform->SetPosition(_vec3(5.f, 100.f, 5.f));
    SetMaterial("SlimeOut_Mtrl", "Body", RENDER_ID::Render_NonAlpha);
    SetMaterial("SlimeIn_Mtrl", "Head", RENDER_ID::Render_Alpha);

    Bones["LArm"]->SetDead();
    Bones["RArm"]->SetDead();
    Bones["LLeg"]->SetDead();
    Bones["RLeg"]->SetDead();

    //head
    Scale = 0.5f;
    SetScale(_vec3(7.f * Scale, 7.f * Scale, 7.f * Scale), "Head");
    SetPosition(_vec3(0.f * Scale, 0.f * Scale, 0.f * Scale), "Head");
    //body
    SetScale(_vec3(10.f * Scale, 10.f * Scale, 10.f * Scale), "Body");

}

void Slime::InitTree()
{
}

void Slime::InitAnimation()
{
}

void Slime::PlayAnimation(_float dt)
{
}

void Slime::PlayIdle(_float dt)
{
}

void Slime::PlayWalk(_float dt)
{
}

void Slime::PlayAttack(_float dt)
{
}

void Slime::PlayDie(_float dt)
{
}

void Slime::OnCollisionStay(Object* other)
{
}

void Slime::Free()
{
    Monster::Free();
}
