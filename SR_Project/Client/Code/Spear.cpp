#include "pch.h"
#include "Spear.h"
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

Spear::Spear(ObjectManager* owner, ObjectType objType) : Item(owner, objType) {}

Spear::~Spear() {}

void Spear::Free()
{
	Item::Free();
}

Spear* Spear::Create(ObjectManager* owner, ObjectType objType)
{
    Spear* Instance = new Spear(owner, objType);
    if (FAILED(Instance->Ready_Object(owner, objType))) {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT Spear::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    if (FAILED(Item::Ready_Object(owner, objType)))
        return E_FAIL;

    itemType = ItemType::Spear;
    auto info = GetComponent<InfoComponent<ItemInfo>>();
    auto i = info->GetInfo();
    i.value = 10.f;
    info->SetInfo(i);

    SetMesh("Cube_Mesh");
    SetMaterial("spear_Mtrl");
    SetRenderId(renderId);

    PlayerSpearInfo();
    ApplyComponents();

    /////////////////////////////////////////////////
    auto physics = AddComponent<PhysicsComponent>();
    physics->SetKinematic(true);//Disable Gravity
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);

    return S_OK;
}

void Spear::Update(_float dt)
{
    Item::Update(dt);
}

void Spear::Late_Update(_float dt)
{
    Item::Late_Update(dt);
}

void Spear::SetCollisionEnter(Object* other)
{
    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();

    if (objType == ObjectType::Monster &&
        static_cast<Player*>(ownerObject)->GetPlayerState() == Player::ePlayerState::ATTACK)
    {
        float spearAttackDamage = ownerObject->GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().power + GetComponent<InfoComponent<ItemInfo>>()->GetInfo().value;

        auto monster = static_cast<Monster*>(other);
        monster->SetHit(true);
        monster->Hit(monster->GetComponent<TransformComponent>()->GetPosition() - ownerObject->GetComponent<TransformComponent>()->GetPosition(), spearAttackDamage);
        other->GetComponent<InfoComponent<EnemyInfo>>()->AddHp(-spearAttackDamage);
    }
}

void Spear::PlayerSpearInfo()
{
    SetScale(1.f);
    SetScaleRatio(_vec3(0.1f, 2.5f, 2.5f));

    SetPosition(_vec3(0.f, 0.5f, 1.9f));
    SetPivot(true, _vec3(0.f, 1.f, -0.4f));
    SetRotation(_vec3(0.8f, 0.f, 0.f));

    SetOwnerObject(owner->GetFrontObject(ObjectType::Player));
    SetRenderId(Engine::RENDER_ID::Render_Alpha);

    auto collision = AddComponent<CollisionComponent>();
    collision->SetLayer(LAYER_PLAYER);
    collision->SetMask(LAYER_ENEMY);
    collision->SetSize(_vec3(0.1f, 1.f, 6.f));
    collision->SetCollisionEnter([this](Object* other) {this->SetCollisionEnter(other); });
}
