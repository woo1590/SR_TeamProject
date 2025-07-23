#include "EnginePCH.h"
#include "MiniMapObject.h"
#include "MiniMapRenderer.h"
#include "TransformComponent.h"
#include "ObjectManager.h"

MiniMapObject::MiniMapObject(ObjectManager* owner) : Object(owner, ObjectType::MiniMap)
{
}

MiniMapObject::~MiniMapObject()
{
    Safe_Release(miniMapRenderer);
}

MiniMapObject* MiniMapObject::Create(ObjectManager* owner)
{
    MiniMapObject* instance = new MiniMapObject(owner);

    if (FAILED(instance->Ready_Object()))
    {
        Safe_Release(instance);
        return nullptr;
    }

    return instance;
}

HRESULT MiniMapObject::Ready_Object()
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition({ 0.f, 0.f, 0.f });
    miniMapRenderer = AddComponent<MiniMapRenderer>(RENDER_ID::Render_NonAlpha);
    owner->AddObject(ObjectType::MiniMap, this);
    return S_OK;
}