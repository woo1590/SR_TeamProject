#include "pch.h"
#include "Bow.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"

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
    Item::Ready_Object(owner, objType);
    auto transform = GetComponent<TransformComponent>();
    auto mesh = GetComponent<MeshRenderer>();
    auto info = GetComponent<InfoComponent<ItemInfo>>();PlayerInfo;
    float Scale = 1.f;
    transform->SetScale(0.1f * Scale, 1.f * Scale, 1.f * Scale);
    transform->SetPosition(0.25f * Scale, -1.2f * Scale, -0.2f * Scale);
    transform->SetPivot(_vec3(-0.25f * Scale, 0.8f * Scale, -0.2f * Scale));
    transform->SetPivotEnable(true);
    transform->SetRotate({ 2.2f,0.f,0.f });

    mesh->SetMaterial(L"bow_Mtrl");
    mesh->SetRenderID(Engine::RENDER_ID::Render_Alpha);

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
