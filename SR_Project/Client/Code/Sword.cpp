#include "pch.h"
#include "Sword.h"

#include "ObjectManager.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"
#include "CollisionComponent.h"

Sword::Sword(ObjectManager* owner, ObjectType objType) : Item(owner, objType) {}

Sword::~Sword() {}
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
    

    auto collision = AddComponent<CollisionComponent>();
    collision->SetLayer(CollisionComponent::LAYER_PLAYER);
    collision->SetMask(CollisionComponent::LAYER_ENEMY);
    collision->SetSize(_vec3(5.f, 5.f, 5.f));
    collision->SetCollisionEnter([this](Object* other) {this->SetCollisionEnter(other); });

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

void Sword::Free()
{
    Item::Free();
}

void Sword::SetCollisionEnter(Object* other)
{
    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();

    if (objType == ObjectType::Monster) {
        float swordAttackDamage = GetComponent<InfoComponent<ItemInfo>>()->GetInfo().attackDamage;
        other->GetComponent<InfoComponent<EnemyInfo>>()->AddHp(-swordAttackDamage);
        collision->ResolveAABBColiision(other);
    }
}
