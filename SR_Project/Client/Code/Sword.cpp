#include "pch.h"
#include "Sword.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "ObjectManager.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "PhysicsComponent.h"
#include "InfoComponent.h"
#include "CollisionComponent.h"
#include "Player.h"
#include "Monster.h"
#include "StatikkEffect.h"

#include "EngineCore.h"
#include "SoundManager.h"
#include "Creeper.h"

Sword::Sword(ObjectManager* owner, ObjectType objType) : Item(owner, objType) {}

Sword::~Sword() {}

void Sword::Free()
{
    Item::Free();
}
Sword* Sword::Create(ObjectManager* owner, ObjectType objType)
{
    Sword* Instance = new Sword(owner, objType);
    if (FAILED(Instance->Ready_Object(owner, objType))) {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT Sword::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    if (FAILED(Item::Ready_Object(owner, objType)))
        return E_FAIL;

    itemType = ItemType::Sword;
    auto info = GetComponent<InfoComponent<ItemInfo>>();
    auto i = info->GetInfo();
    i.value = 10.f;
    info->SetInfo(i);

    auto skillInfo = AddComponent<InfoComponent<SkillInfo>>();
    auto ii = skillInfo->GetInfo();
    ii.level = 1;
    ii.MaxTarget = 3;
    ii.DamagePercent = 0.5f;
    ii.SkillRange = 20.f;
    skillInfo->SetInfo(ii);


    SetMesh("Cube_Mesh");
    SetMaterial("sword_Mtrl");
    SetRenderId(renderId);

    PlayerSwordInfo();
    ApplyComponents();

    /////////////////////////////////////////////////
    auto physics = AddComponent<PhysicsComponent>();
    physics->SetKinematic(true);//Disable Gravity
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);

    return S_OK;
}

void Sword::Update(_float dt)
{
    Item::Update(dt);
    
    auto player = static_cast<Player*>(ownerObject);
    if (!hitMonsters.empty() || player->GetPlayerState() == Player::ePlayerState::ATTACK)
    {
        attackTime += dt;

        if (player->GetPlayerState() != Player::ePlayerState::ATTACK)
            hitMonsters.clear();
        if (attackTime >= player->GetAttackDelay())
            hitMonsters.clear();
    }

    if (targetMonsters.empty()) 
        return;
    
    delayTimer += dt;
    int target = static_cast<int>(delayTimer / damageTerm);

    if (target <= preTarget) 
        return;

    if (target >= targetMonsters.size())
    {
        prePos = { 0.f,0.f,0.f };
        curPos = { 0.f,0.f,0.f };
        targetMonsters.clear();
        delayTimer = 0.f;
        preTarget = 0;
        return;
    }

    if (!targetMonsters.at(target)) 
        return;

    float playerPower = ownerObject->GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().power;
    auto skillInfo = GetComponent<InfoComponent<SkillInfo>>()->GetInfo();

    auto pEnemyInfo = targetMonsters.at(target)->GetComponent<InfoComponent<EnemyInfo>>();
    if (!pEnemyInfo) 
        return;

    auto skillDamage = playerPower * skillInfo.DamagePercent;

    static_cast<Monster*>(targetMonsters.at(target))->Hit(curPos - prePos, skillDamage);
    pEnemyInfo->AddHp( -skillDamage);

    curPos = targetMonsters.at(target)->GetComponent<TransformComponent>()->GetWorldPosition();
    auto statikkEffect = StatikkEffect::Create(owner, ObjectType::ParticleEffect, prePos, curPos);
    owner->AddObject(ObjectType::ParticleEffect, statikkEffect);

    prePos = curPos;
    preTarget = target;
}

void Sword::Late_Update(_float dt)
{
    Item::Late_Update(dt);
}

void Sword::SetCollisionEnter(Object* other)
{
    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();
    auto player = static_cast<Player*>(ownerObject);
    if (objType == ObjectType::Monster && 
        player->GetPlayerState() == Player::ePlayerState::ATTACK)
    {
        if (other->GetObjectType() == ObjectType::Bone) return;
        for (auto& m : hitMonsters)
        {
            if (m == other || dynamic_cast<Creeper*>(m))
            {
                return;
            }
        }
        hitMonsters.push_back(other);

        float playerPower = ownerObject->GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().power;
        float weaponValue = GetComponent<InfoComponent<ItemInfo>>()->GetInfo().value;
        float swordAttackDamage = playerPower + weaponValue;

        auto targetMonster = static_cast<Monster*>(other);
        targetMonster->SetHit(true);
        targetMonster->Hit(targetMonster->GetComponent<TransformComponent>()->GetPosition() - ownerObject->GetComponent<TransformComponent>()->GetPosition(), swordAttackDamage);
        other->GetComponent<InfoComponent<EnemyInfo>>()->AddHp(-swordAttackDamage);

        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("HitSword");

        if (player->IsStatikkMode())
        {
            auto skillInfo = GetComponent<InfoComponent<SkillInfo>>()->GetInfo();
            targetMonsters.push_back(targetMonster);

            FindNextTarget(targetMonster);

            auto skillDamage = playerPower * skillInfo.DamagePercent;
            
            targetMonster->Hit(targetMonster->GetComponent<TransformComponent>()->GetPosition() - ownerObject->GetComponent<TransformComponent>()->GetPosition(), skillDamage);
            targetMonster->GetComponent<InfoComponent<EnemyInfo>>()->AddHp(-skillDamage);

            curPos = targetMonster->GetComponent<TransformComponent>()->GetWorldPosition();
            auto playerPos = ownerObject->GetComponent<TransformComponent>()->GetWorldPosition();
            
            auto statikkEffect = StatikkEffect::Create(owner, ObjectType::ParticleEffect, playerPos, curPos);
            owner->AddObject(ObjectType::ParticleEffect, statikkEffect);
            prePos = curPos;

            delayTimer = 0.f;
            player->SetStatikkMode(false);

            EngineCore::GetInstance()->GetSoundManager()->PlaySFX("Statikk");
        }
    }
}

void Sword::PlayerSwordInfo()
{
    SetScale(1.f);
    SetScaleRatio(_vec3(0.1f, 2.f, 2.f));
    SetPosition(_vec3(0.f, 0.2f, 1.f));
    SetPivot(true, _vec3(0.f, 0.8f, 0.f));
    SetRotation(_vec3(0.8f, 0.f, 0.f));
    SetOwnerObject(owner->GetFrontObject(ObjectType::Player));
    SetRenderId(Engine::RENDER_ID::Render_Alpha);

    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<OBBCollider>();
    collision->SetLayer(LAYER_PLAYER);
    collision->SetMask(LAYER_ENEMY);
    collision->SetSize(_vec3(0.1f, 1.f, 6.f));
    collision->SetCollisionEnter([this](Object* other) {this->SetCollisionEnter(other); });
}

void Sword::FindNextTarget(Object* targetMonster)
{
    auto skillInfo = GetComponent<InfoComponent<SkillInfo>>()->GetInfo();
    if (targetMonsters.size() >= skillInfo.MaxTarget) return;

    auto targetPos = targetMonster->GetComponent<TransformComponent>()->GetWorldPosition();
    float distance = skillInfo.SkillRange;
    Object* nextTargetMonster = nullptr;

    auto monsters = owner->GetObjectList(ObjectType::Monster);
    for (auto& monster : monsters)
    {
        if (monster->GetObjectType() == ObjectType::Bone) continue;
        _bool includedMonster = false;
        for (auto& m : targetMonsters)
        {
            if (m == monster || dynamic_cast<Creeper*>(m))
            {
                includedMonster = true;
                break;
            }
        }
        if (includedMonster) continue;

        _vec3 distanceVec = monster->GetComponent<TransformComponent>()->GetWorldPosition() - targetPos;
        float curDistance = sqrtf(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y + distanceVec.z * distanceVec.z);
        if (curDistance < distance)
        {
            distance = curDistance;
            nextTargetMonster = monster;
        }
    }
    if (nextTargetMonster)
    {
        targetMonsters.push_back(nextTargetMonster);
        FindNextTarget(nextTargetMonster);
    }
}
