#include "pch.h"
#include "Tnt.h"

#include "Scene.h"
#include "PhysicsSystem.h"
#include "ObjectManager.h"
#include "InfoComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"

#include "Player.h"

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
    i.value = 10.f;
    info->SetInfo(i);

    SetMesh("Cube_Mesh");
    SetMaterial("tnt_Mtrl");
    SetRenderId(renderId);

    TntInfo();

    ApplyComponents();

    /////////////////////////////////////////////////
    auto physics = AddComponent<PhysicsComponent>();
    physics->SetMass(1.f);
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

        if (ownerObject->GetObjectType() == ObjectType::Player && static_cast<Player*>(ownerObject)->GetPlayerState() == Player::ePlayerState::ATTACK)
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
            //데미지 로직
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
    collision->SetLayer(CollisionComponent::LAYER_PLAYER);
    collision->SetMask(CollisionComponent::LAYER_DEFAULT);
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
    velocity.z = throwDirection.z * throwSpeed;
    physics->SetVelocity(velocity);
}
