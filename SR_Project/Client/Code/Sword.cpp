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
    auto info = GetComponent<InfoComponent<ItemInfo>>();
    ItemInfo i;
    i.size = 1.f;
    i.scale = _vec3{ 0.1f, 2.f, 2.f };
    i.position = _vec3{ 0.f, 0.2f, 1.f };
    i.pivotEnable = true;
    i.pivot = _vec3{ 0.f, 0.8f, 0.f };
    i.rotation = _vec3{ 0.8f, 0.f, 0.f };
    i.meshType = L"Cube_Mesh";
    i.material = L"sword_Mtrl";
    i.renderId = Engine::RENDER_ID::Render_Alpha;
    info->SetInfo(i);

    auto transform = GetComponent<TransformComponent>();
    transform->SetScale(i.scale.x * i.size, i.scale.y * i.size, i.scale.z * i.size);
    transform->SetPosition(i.position.x * i.size, i.position.y * i.size, i.position.z * i.size);
    transform->SetPivot(_vec3(i.pivot.x * i.size, i.pivot.y * i.size, i.pivot.z * i.size));
    transform->SetPivotEnable(i.pivotEnable);
    transform->SetRotate({ i.rotation.x ,i.rotation.y ,i.rotation.z });

    auto mesh = GetComponent<MeshRenderer>();
    mesh->SetMesh(i.meshType);
    mesh->SetMaterial(i.material);
    mesh->SetRenderID(i.renderId);

    auto collision = AddComponent<CollisionComponent>();
    collision->SetLayer(CollisionComponent::LAYER_PLAYER);
    collision->SetMask(CollisionComponent::LAYER_ENEMY);
    collision->SetSize(_vec3(2.f, 2.f, 5.f));
    collision->SetCollisionEnter([this](Object* other) {this->SetCollisionEnter(other); });

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
    }
}
