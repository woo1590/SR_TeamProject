#include "pch.h"
#include "Bow.h"

#include "ObjectManager.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"
#include "CollisionComponent.h"

Bow::Bow(ObjectManager* owner, ObjectType objType) : Item(owner, objType) {}
Bow::~Bow(){}

Bow* Bow::Create(ObjectManager* owner, ObjectType objType)
{
    Bow* Instance = new Bow(owner, objType);
    if (FAILED(Instance->Ready_Object(owner, objType))) {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT Bow::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    if (FAILED(Item::Ready_Object(owner, objType)))
        return E_FAIL;
    
    auto collision = AddComponent<CollisionComponent>();
    collision->SetLayer(CollisionComponent::LAYER_PLAYER);
    collision->SetMask(CollisionComponent::LAYER_ENEMY);
    collision->SetSize(_vec3(5.f, 5.f, 5.f));

    return S_OK;
}

void Bow::Update(_float dt)
{
    Item::Update(dt);
}

void Bow::Late_Update(_float dt)
{
    Item::Late_Update(dt);
}

void Bow::Free()
{
    Item::Free();
}
