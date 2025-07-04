#include "pch.h"
#include "Sword.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"

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
    Item::Ready_Object(owner, objType);
    auto transform = GetComponent<TransformComponent>();
    auto mesh = GetComponent<MeshRenderer>();
    auto info = GetComponent<InfoComponent<ItemInfo>>();

    float Scale = 1.f;
    transform->SetScale(0.1f * Scale, 2.f * Scale, 2.f * Scale);
    transform->SetPosition(0.f, 0.2f, 1.f * Scale);
    transform->SetPivot(_vec3(0.f * Scale, 0.8f * Scale, 0.f * Scale));
    transform->SetPivotEnable(true);
    transform->SetRotate({ 0.8f,0.f,0.f });

    mesh->SetMaterial(L"sword_Mtrl");
    mesh->SetRenderID(Engine::RENDER_ID::Render_Alpha);
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
