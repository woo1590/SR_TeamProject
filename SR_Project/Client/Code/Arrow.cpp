#include "pch.h"
#include "Arrow.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"
#include "ObjectManager.h"
#include "CollisionComponent.h"

Arrow::Arrow(ObjectManager* owner, ObjectType objType) : Item(owner, objType) 
{
}
Arrow::~Arrow() 
{
}
void Arrow::Free()
{
	Item::Free();
}

Arrow* Arrow::Create(ObjectManager* owner, ObjectType objType, Object* shooter, _vec3 normalDirection)
{
    Arrow* Instance = new Arrow(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType, shooter, normalDirection)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Arrow::Ready_Object(ObjectManager* owner, ObjectType objType, Object* shooter, _vec3 normalDirection)
{
    if (FAILED(Item::Ready_Object(owner,objType)))
        return E_FAIL;

    SetOwnerObject(shooter);
    arrowDirection = normalDirection;

    auto info = GetComponent<InfoComponent<ItemInfo>>();
    auto i = info->GetInfo();
    i.attackDamage = 10.f;
    info->SetInfo(i);

    SetMesh(L"Cube_Mesh");
    SetMaterial(L"arrow_Mtrl");
    SetRenderId(renderId);

    switch (ownerObject->GetObjectType()) 
    {
    case ObjectType::Player:
        PlayerArrowInfo();
        break;
    case ObjectType::Monster:
        MonsterArrowInfo();
        break;
    }

    ApplyComponents();

    return S_OK;
}

void Arrow::Update(_float dt)
{
    Item::Update(dt);
    
    auto transform = GetComponent<TransformComponent>();
    auto info = GetComponent<InfoComponent<ItemInfo>>()->GetInfo();

    if (hitObject == nullptr)
        transform->Translate(arrowDirection * arrowSpeed * dt);

    else 
    {
        hitTime += dt;
        _vec3 curPos = hitObject->GetComponent<TransformComponent>()->GetWorldPosition();
        transform->Translate(curPos - hitObjectPos);
        hitObjectPos = curPos;

        if (hitTime > arrowPersistTime) 
        {
            SetDead();
        }
    }
}

void Arrow::Late_Update(_float dt)
{
    Item::Late_Update(dt);
}

void Arrow::SetCollisionEnter(Object* other)
{
    if (hitObject != nullptr) return;
    ObjectType objType = other->GetObjectType();
    arrowSpeed = 0.f;

    hitObject = other;
    hitObjectPos = other->GetComponent<TransformComponent>()->GetWorldPosition();
    auto collision = GetComponent<CollisionComponent>();
    auto info = GetComponent<InfoComponent<ItemInfo>>();
    switch (objType) 
    {
    case ObjectType::Monster:
        other->GetComponent<InfoComponent<EnemyInfo>>()->AddHp(info->GetInfo().attackDamage);
        collision->ResolveAABBColiision(other);
        break;
    case ObjectType::StaticBlock:
        collision->ResolveAABBColiision(other);
        break;
    }
}

void Arrow::ArrowRotateSet()
{
    auto transform = GetComponent<TransformComponent>();
    transform->SetForward(arrowDirection);
    
    _vec3 right = transform->GetRight();
    
    float angle = D3DXToRadian(-45.f);
    
    _matrix rotX, rotZ , matRot;
    D3DXMatrixRotationAxis(&rotX, &right, angle);
    D3DXMatrixRotationAxis(&rotZ, &arrowDirection, angle);
    matRot = rotX * rotZ;
    
    _vec3 finalDir;
    D3DXVec3TransformNormal(&finalDir, &arrowDirection, &matRot);
    D3DXVec3Normalize(&finalDir, &finalDir);

    _float pitch = sqrtf(2.f) * asinf(finalDir.y);
    pitch *= arrowDirection.z;
    _float yaw = atan2f(-arrowDirection.x, arrowDirection.z);
    _float roll = sqrtf(2.f) * asinf(finalDir.y);
    roll *= arrowDirection.x;
    SetRotation(_vec3(-pitch, -yaw, roll));

}

void Arrow::PlayerArrowInfo()
{
    arrowSpeed = 50.f;

    SetScale(1.f);
    SetScaleRatio(_vec3(0.1f, 1.f, 1.f));
    SetPosition(ownerObject->GetComponent<TransformComponent>()->GetWorldPosition());
    SetPivot(false);
    SetRenderId(Engine::RENDER_ID::Render_Alpha);

    ArrowRotateSet();

    auto collision = GetComponent<CollisionComponent>();
    collision->SetLayer(CollisionComponent::LAYER_PLAYER);
    collision->SetMask(CollisionComponent::LAYER_ENEMY | CollisionComponent::LAYER_DEFAULT);
    collision->SetCollisionEnter([this](Object* other) {this->SetCollisionEnter(other); });
}

void Arrow::MonsterArrowInfo()
{
    arrowSpeed = 50.f;

    SetScale(1.f);
    SetScaleRatio(_vec3(0.1f, 1.f, 1.f));
    SetPosition(ownerObject->GetComponent<TransformComponent>()->GetWorldPosition());
    SetPivot(false);
    SetRenderId(Engine::RENDER_ID::Render_Alpha);

    ArrowRotateSet();

    auto collision = GetComponent<CollisionComponent>();
    collision->SetLayer(CollisionComponent::LAYER_ENEMY);
    collision->SetMask(CollisionComponent::LAYER_PLAYER | CollisionComponent::LAYER_DEFAULT);
    collision->SetCollisionEnter([this](Object* other) {this->SetCollisionEnter(other); });
}
