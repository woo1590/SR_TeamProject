#include "pch.h"
#include "Item.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"

Item::Item(ObjectManager* owner, ObjectType objType) :Object(owner, objType) {}
Item::~Item() {}

Item* Item::Create(ObjectManager* owner, ObjectType objType)
{
    Item* Instance = new Item(owner, objType);
    if (FAILED(Instance->Ready_Object(owner, objType))){
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT Item::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    if (FAILED(Object::Ready_Object()))
        return E_FAIL;
    auto transform = AddComponent<TransformComponent>();
    auto info = AddComponent<InfoComponent<ItemInfo>>();
    auto mesh = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    owner->AddObject(objType, this);
    
    return S_OK;
}

void Item::Update(_float dt)
{
    Object::Update(dt);
}

void Item::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Item::Free()
{
    Object::Free();
}
