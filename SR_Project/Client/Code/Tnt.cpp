#include "pch.h"
#include "Tnt.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "ObjectManager.h"

#include "InfoComponent.h"
#include "MeshRendererComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "CameraManager.h"
#include "CollisionComponent.h"

#include "Player.h"
#include "Monster.h"

#include "EngineCore.h"
#include "SoundManager.h"
#include "Creeper.h"
#include "ThirdcamComponent.h"
#include "CameraComponent.h"
#include "ExplodeEffect.h"

Tnt::Tnt(ObjectManager* owner, ObjectType objType) : Item(owner, objType){}

Tnt::~Tnt() {}

void Tnt::Free()
{
	Item::Free();
}

Tnt* Tnt::Create(ObjectManager* owner, ObjectType objType)
{
    Tnt* Instance = new Tnt(owner, objType);
    if (FAILED(Instance->Ready_Object(owner, objType))) {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT Tnt::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    if (FAILED(Item::Ready_Object(owner, objType)))
        return E_FAIL;

    auto info = GetComponent<InfoComponent<ItemInfo>>();
    auto i = info->GetInfo();
    i.value = 50.f;
    info->SetInfo(i);

    SetMesh("Cube_Mesh");
    SetMaterial("tnt_Mtrl");
    SetRenderId(renderId);

    TntInfo();

    ApplyComponents();

    /////////////////////////////////////////////////
    auto physics = AddComponent<PhysicsComponent>();
    physics->SetMass(0.2f);
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);

    return S_OK;
}

void Tnt::Update(_float dt)
{
    Item::Update(dt);
    if (ownerObject != nullptr)
    {
        auto transform = GetComponent<TransformComponent>();
        auto ownerTransform = ownerObject->GetComponent<TransformComponent>();
        transform->SetPosition(ownerTransform->GetWorldPosition() + tntOffset);

        if (ownerObject->GetObjectType() == ObjectType::Player && (static_cast<Player*>(ownerObject)->GetPlayerState() == Player::ePlayerState::ATTACK || static_cast<Player*>(ownerObject)->IsMovingToAttack() == true))
        {
            ThrowTnt();
        }
    }
    if (throwTnt)
    {
        TntTime += dt;
        auto physics = GetComponent<PhysicsComponent>();
        if (physics->IsGrounded() == true) 
        {
            _vec3 velocity = { 0.f,0.f,0.f };
            physics->SetVelocity(velocity);
        }
        if (TntTime >= TntBoom)
        {
            auto transform = GetComponent<TransformComponent>();
            auto pos = transform->GetWorldPosition();
            auto TntDamage = GetComponent<InfoComponent<ItemInfo>>()->GetInfo().value;
            
            auto player = owner->GetFrontObject(ObjectType::Player);
            auto playerPos = player->GetComponent<TransformComponent>()->GetWorldPosition();
            
            _vec3 distanceVec = playerPos - pos;
            _float distance = sqrtf(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y + distanceVec.z * distanceVec.z);
            if (distance <= TntRange)
            {
                player->GetComponent<InfoComponent<PlayerInfo>>()->AddHp(-TntDamage);
            }
            
            auto monsters = owner->GetObjectList(ObjectType::Monster);
            for (auto& monster : monsters)
            {
                if (monster->GetObjectType() == ObjectType::Bone) continue;
                if (dynamic_cast<Creeper*>(monster))
                    continue;

                auto monsterPos = monster->GetComponent<TransformComponent>()->GetWorldPosition();
            
                distanceVec = monsterPos - pos;
                distance = sqrtf(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y + distanceVec.z * distanceVec.z);
                if (distance <= TntRange)
                {
                    auto mon = dynamic_cast<Monster*>(monster);
                    mon->SetHit(true);
                    mon->Hit(monster->GetComponent<TransformComponent>()->GetPosition() - GetComponent<TransformComponent>()->GetPosition(), TntDamage);
                    monster->GetComponent<InfoComponent<EnemyInfo>>()->AddHp(-TntDamage);
                }
            }
            EngineCore::GetInstance()->GetSoundManager()->PlaySFX("BoomTNT");
            auto cam = GetScene()->GetCameraManager()->GetMainCamera()->GetOwner()->GetComponent<ThirdcamComponent>();
            if (cam)
                cam->SetShake(15.f, 0.5f);

            auto effect = ExplodeEffect::Create(owner, ObjectType::ParticleEffect);
            effect->GetComponent<TransformComponent>()->SetPosition(GetComponent<TransformComponent>()->GetPosition());
            effect->SetDeadTime(1.f);
            owner->AddObject(ObjectType::ParticleEffect, effect);

            SetDead();
        }
    }
}

void Tnt::Late_Update(_float dt)
{
    Item::Late_Update(dt);
}

void Tnt::TntToPlayer(Object* player)
{
    ownerObject = player;
    GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetWorldPosition() + tntOffset);
    auto physics = GetComponent<PhysicsComponent>();
    physics->SetGround(true);
}

void Tnt::TntInfo()
{
    SetScale(1.f);
    SetScaleRatio(_vec3(1.f, 1.f, 1.f));
    SetPosition(_vec3(0.f, 0.f, 0.f));
    SetPivot(true, _vec3(0.f, 0.8f, 0.f));
    SetRotation(_vec3(0.f, 0.f, 0.f));
    SetOwnerObject(owner->GetFrontObject(ObjectType::Player));
    SetRenderId(Engine::RENDER_ID::Render_Alpha);

    auto collision = AddComponent<CollisionComponent>();
    collision->SetLayer(LAYER_PLAYER);
    collision->SetMask(LAYER_DEFAULT);
    collision->SetSize(_vec3(1.f, 1.f, 1.f));
}

void Tnt::ThrowTnt()
{
    throwTnt = true;
    throwDirection = static_cast<Player*>(ownerObject)->GetAttackDirection();
    D3DXVec3Normalize(&throwDirection, &throwDirection);
    throwDirection.y = 0;
    ownerObject = nullptr;
    auto physics = GetComponent<PhysicsComponent>();
    physics->SetGround(false);

    _vec3 velocity = physics->GetVelocity();
    velocity.x = throwDirection.x * throwSpeed;
    velocity.y = 10.f;
    velocity.z = throwDirection.z * throwSpeed;
    physics->SetVelocity(velocity);
}
