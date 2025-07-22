#include "pch.h"
#include "ShulkerBullet.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "ObjectManager.h"
#include "CollisionComponent.h"
#include "EngineCore.h"
#include "PhysicsComponent.h"
#include "Bone.h"
#include "InfoComponent.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "Player.h"

ShulkerBullet::ShulkerBullet(ObjectManager* owner, ObjectType objType)
	:Object(owner, objType)
{
}

ShulkerBullet::~ShulkerBullet()
{
}

ShulkerBullet* ShulkerBullet::Create(ObjectManager* owner, ObjectType objType)
{
    ShulkerBullet* Instance = new ShulkerBullet(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT ShulkerBullet::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<AABBCollider>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetLayer(LAYER_PROJECTILE);
    collision->SetMask(LAYER_PLAYER);
    collision->SetCollisionEnter([this](Object* other) {this->OnCollisionEnter(other); });
    collision->SetSize(_vec3(1.f, 1.f, 1.f));

    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);
    physics->SetKinematic(true);
    physics->SetMass(1.f);

    Bones.push_back(Bone::Create(owner, ObjectType::Bone, _vec3(0.4f, 0.4f, 0.1f), this, "ShulkerBullet_Mtrl"));
    Bones.push_back(Bone::Create(owner, ObjectType::Bone, _vec3(0.4f, 0.4f, 0.1f), this, "ShulkerBullet_Mtrl"));
    Bones.push_back(Bone::Create(owner, ObjectType::Bone, _vec3(0.4f, 0.4f, 0.1f), this, "ShulkerBullet_Mtrl"));
    
    Bones[0]->GetComponent<TransformComponent>()->SetRotate(0.f, D3DXToRadian(- 90.f), 0.f);
    Bones[1]->GetComponent<TransformComponent>()->SetRotate(D3DXToRadian(-90.f), 0.f, 0.f);
    Bones[2]->GetComponent<TransformComponent>()->SetRotate(0.f, 0.f, 0.f);
    
    for (auto& bone : Bones)
        owner->AddObject(ObjectType::Bone, bone);

    SetActive(true);
    return S_OK;
}

void ShulkerBullet::Update(_float dt)
{
    Object::Update(dt);
    if (Active)
    {
        ActiveTimer += dt;
    
        if (ActiveTimer > 8.f) SetActive(false);
        
        auto transform = GetComponent<TransformComponent>();
        _vec3 rot = transform->GetRotate();
        transform->SetRotate(rot.x + dt* 4.f, rot.y + dt * 8.f, rot.z + dt * 2.f);
        
    }
    CheckDir();
    MoveToDir(dt);
}

void ShulkerBullet::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void ShulkerBullet::SetActive(bool active)
{
    Active = active;
    if (Active)
    {
        for (auto& bone : Bones)
        {
            bone->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_NonAlpha);
            GetComponent<CollisionComponent>()->SetSize(_vec3(1.f, 1.f, 1.f));

            RotateCount = 0;
            ActiveTimer = 0.f;
        }
    }
    else
    {
        for (auto& bone : Bones)
        {
            bone->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_None);
            GetComponent<CollisionComponent>()->SetSize(_vec3(0.f, 0.f, 0.f));

            RotateCount = 0;
            ActiveTimer = 0.f;
        }
    }
}

void ShulkerBullet::CheckDir()
{
    if (!Active) return;

    auto player = owner->GetFrontObject(ObjectType::Player);
    _vec3 playerpos = player->GetComponent<TransformComponent>()->GetPosition();

    _vec3 selfpos = GetComponent<TransformComponent>()->GetPosition();

    _vec3 dir = playerpos - selfpos;

    if (abs(dir.x) < abs(dir.z))
    {
        if (dir.z > 0) CurDir = U;
        else CurDir = D;
    }
    else
    {
        if (dir.x > 0) CurDir = R;
        else CurDir = L;
    }

    if (PreDir != CurDir)
    {
        ++RotateCount;
        PreDir = CurDir;
    }
}

void ShulkerBullet::MoveToDir(_float dt)
{
    if (!Active) return;
    //if (RotateCount >= 3) SetActive(false);

    auto transform = GetComponent<TransformComponent>();
    switch (CurDir)
    {
    case L:
        transform->Translate(_vec3(-5.f * dt, 0.f, 0.f));
        break;
    case R:
        transform->Translate(_vec3(5.f * dt, 0.f, 0.f));
        break;
    case U:
        transform->Translate(_vec3(0.f, 0.f, 5.f * dt));
        break;
    case D:
        transform->Translate(_vec3(0.f, 0.f, -5.f * dt));
        break;
    }
}

void ShulkerBullet::OnCollisionEnter(Object* other)
{
    if (!Active) return;        //!active -> nocollision

    ObjectType objType = other->GetObjectType();
    if (objType == ObjectType::Player)
    {
        auto playerStat = other->GetComponent<InfoComponent<PlayerInfo>>();
        playerStat->AddHp(-20);

        //player floating//
        auto player = dynamic_cast<Player*>(other);
        player->SetFloatMode(true);
        ///////////////////

        SetActive(false);
    }
}

void ShulkerBullet::Free()
{
    Object::Free();
}
