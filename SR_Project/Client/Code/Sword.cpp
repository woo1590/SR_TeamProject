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

    auto info = GetComponent<InfoComponent<ItemInfo>>();
    auto i = info->GetInfo();
    i.value = 10.f;
    info->SetInfo(i);

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
}

void Sword::Late_Update(_float dt)
{
    Item::Late_Update(dt);
}

void Sword::SetCollisionEnter(Object* other)
{
    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();

    if (objType == ObjectType::Monster && 
        static_cast<Player*>(ownerObject)->GetPlayerState() == Player::ePlayerState::ATTACK && 
        static_cast<Monster*>(other)->GetHit() == false) 
    {
        float swordAttackDamage = ownerObject->GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().power + GetComponent<InfoComponent<ItemInfo>>()->GetInfo().value;

        auto monster = static_cast<Monster*>(other);
        monster->SetHit(true);
        monster->Hit(monster->GetComponent<TransformComponent>()->GetPosition() - GetComponent<TransformComponent>()->GetPosition(), swordAttackDamage);
        other->GetComponent<InfoComponent<EnemyInfo>>()->AddHp(-swordAttackDamage);
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
    collision->SetLayer(CollisionComponent::LAYER_PLAYER);
    collision->SetMask(CollisionComponent::LAYER_ENEMY);
    collision->SetCollisionEnter([this](Object* other) {this->SetCollisionEnter(other); });
}
