#include "pch.h"
#include "Shulker.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "InfoComponent.h"

Shulker::Shulker(ObjectManager* owner, ObjectType objType)
	:Monster(owner, objType)
{
}

Shulker::~Shulker()
{
}

Shulker* Shulker::Create(ObjectManager* owner, ObjectType objType)
{
    Shulker* Instance = new Shulker(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Shulker::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Monster::Ready_Object(owner, objType);

    InitTransform(objType);

    //Init Collision
    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(3.f, 3.f, 3.f));

    //Create BT
    InitTree();

    //Animation
    InitAnimation();

    GetComponent<InfoComponent<EnemyInfo>>()->SetInfo({ 1,100,100, 0, 0, 15, 0, 6 });
    
    return S_OK;
}

void Shulker::Update(_float dt)
{
    Monster::Update(dt);
}

void Shulker::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void Shulker::Attack(Object* target)
{
}

void Shulker::Die()
{
}

void Shulker::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    transform->SetRotate(_vec3(0.f, D3DXToRadian(90.f), 0.f));

    SetMaterial("ShulkerFace_Mtrl", "Body", RENDER_ID::Render_NonAlpha);
    SetMaterial("ShulkerTop_Mtrl", "Head", RENDER_ID::Render_Alpha);
    Add_Bone("BottomHead", ObjectType::Bone, Bones["Body"], "ShulkerBottom_Mtrl");
    SetMaterial("ShulkerBottom_Mtrl", "BottomHead", RENDER_ID::Render_Alpha);

    SetScale(_vec3(10.f * Scale, 10.f * Scale, 10.f * Scale), "Head");
    SetPosition(_vec3(0.f * Scale, 5.f * Scale, 0.f * Scale), "Head");
    //body
    SetScale(_vec3(5.f * Scale, 5.f * Scale, 5.f * Scale), "Body");
    SetScale(_vec3(10.f * Scale, 10.f * Scale, 10.f * Scale), "BottomHead");
    SetPosition(_vec3(0.f * Scale, -2.f * Scale, 0.f * Scale), "BottomHead");

    Bones["LArm"]->SetDead();
    Bones["LArm"] = nullptr;
    Bones["RArm"]->SetDead();
    Bones["RArm"] = nullptr;
    Bones["LLeg"]->SetDead();
    Bones["LLeg"] = nullptr;
    Bones["RLeg"]->SetDead();
    Bones["RLeg"] = nullptr;
}

void Shulker::InitTree()
{
}

void Shulker::InitAnimation()
{
}

void Shulker::PlayAnimation(_float dt)
{
}

void Shulker::PlayIdle(_float dt)
{
}

void Shulker::PlayDie(_float dt)
{
}

void Shulker::Free()
{
}
