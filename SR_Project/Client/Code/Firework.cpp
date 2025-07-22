#include "pch.h"
#include "Firework.h"
#include "Scene.h"
#include "PhysicsSystem.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"
#include "ObjectManager.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"

#include "Monster.h"

#include "EngineCore.h"
#include "SoundManager.h"
#include "Creeper.h"
#include "FireworkEffect.h"

Firework::Firework(ObjectManager* owner, ObjectType objType) : Item(owner, objType)
{
}

Firework::~Firework() 
{
}

void Firework::Free()
{
	Item::Free();
}

Firework* Firework::Create(ObjectManager* owner, ObjectType objType, Object* shooter, _vec3 normalDirection)
{
    Firework* Instance = new Firework(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType, shooter, normalDirection)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Firework::Ready_Object(ObjectManager* owner, ObjectType objType, Object* shooter, _vec3 normalDirection)
{
    if (FAILED(Item::Ready_Object(owner, objType)))
        return E_FAIL;

    SetOwnerObject(shooter);
    fireworkDirection = normalDirection;

    auto physics = AddComponent<PhysicsComponent>();
    physics->SetKinematic(true);
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);

    auto info = GetComponent<InfoComponent<ItemInfo>>();
    auto i = info->GetInfo();
    i.value = 10.f;
    info->SetInfo(i);

    SetMesh("Cube_Mesh");
    SetMaterial("firework_Mtrl");
    SetRenderId(renderId);

    PlayerFireworkInfo();

    ApplyComponents();

    return S_OK;
}

void Firework::Update(_float dt)
{
    Item::Update(dt);

    shootTime += dt;
    if (shootTime > fireworkPersistTime)
    {
        SetDead();
    }

    auto transform = GetComponent<TransformComponent>();
    transform->Translate(fireworkDirection * fireworkSpeed * dt);
}

void Firework::Late_Update(_float dt)
{
    Item::Late_Update(dt);
}

void Firework::SetCollisionEnter(Object* other)
{

    auto transform = GetComponent<TransformComponent>();
    auto pos = transform->GetWorldPosition();
    auto fireworkDamage = GetComponent<InfoComponent<ItemInfo>>()->GetInfo().value;
    
    auto player = owner->GetFrontObject(ObjectType::Player);
    auto playerPos = player->GetComponent<TransformComponent>()->GetWorldPosition();
    
    _vec3 distanceVec = playerPos - pos;
    _float distance = sqrtf(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y + distanceVec.z * distanceVec.z);
    if (distance <= fireworkRange)
    {
        player->GetComponent<InfoComponent<PlayerInfo>>()->AddHp(-fireworkDamage);
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
        if (distance <= fireworkRange)
        {
            auto mon = dynamic_cast<Monster*>(monster);
            if (!mon) continue;
            mon->SetHit(true);
            mon->Hit(monster->GetComponent<TransformComponent>()->GetPosition() - GetComponent<TransformComponent>()->GetPosition(), fireworkDamage);
            monster->GetComponent<InfoComponent<EnemyInfo>>()->AddHp(-fireworkDamage);
        }
    }
    EngineCore::GetInstance()->GetSoundManager()->PlaySFX("BoomFirework");

    for (int i = 0; i < 20; ++i)
    {
        auto fireworkEffect = FireworkEffect::Create(owner, ObjectType::ParticleEffect, this);
        owner->AddObject(ObjectType::ParticleEffect, fireworkEffect);
    }

    SetDead();
}

void Firework::ArrowRotateSet()
{
    _float pitch = sqrtf(2.f) * asinf(fireworkDirection.y);
    pitch *= fireworkDirection.z;
    _float yaw = atan2f(-fireworkDirection.x, fireworkDirection.z);
    _float roll = sqrtf(2.f) * asinf(fireworkDirection.y);
    roll *= fireworkDirection.x;
    SetRotation(_vec3(-pitch, -yaw, roll));
}

void Firework::PlayerFireworkInfo()
{
    fireworkSpeed = 120.f;

    SetScale(1.f);
    SetScaleRatio(_vec3(0.1f, 1.f, 1.f));
    SetPosition(ownerObject->GetComponent<TransformComponent>()->GetWorldPosition());
    SetPivot(false);
    SetRenderId(Engine::RENDER_ID::Render_Alpha);

    ArrowRotateSet();

    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<AABBCollider>();
    collision->SetLayer(LAYER_PROJECTILE);
    collision->SetMask(LAYER_ENEMY | LAYER_DEFAULT);
    collision->SetCollisionEnter([this](Object* other) {this->SetCollisionEnter(other); });
}
