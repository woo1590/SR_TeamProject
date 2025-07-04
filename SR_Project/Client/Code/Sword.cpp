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
    auto info = GetComponent<InfoComponent<ItemInfo>>();
    ItemInfo i;
    i.visible = true;
    i.size = 1.f;
    i.scale = _vec3{ 0.1f, 2.f, 2.f };
    i.position = _vec3{ 0.f, 0.2f, 1.f };
    i.pivotEnable = true;
    i.pivot = _vec3{ 0.f, 0.8f, 0.f };
    i.rotation = _vec3{ 0.8f, 0.f, 0.f };
    i.material = L"sword_Mtrl";
    i.renderId = Engine::RENDER_ID::Render_Alpha;
    info->SetInfo(i);

    auto transform = GetComponent<TransformComponent>();
    transform->SetScale(i.scale.x * i.size, i.scale.y * i.size, i.scale.z * i.size);
    transform->SetPosition(i.position.x * i.size, i.position.y * i.size, i.position.z * i.size);
    transform->SetPivot(_vec3(i.pivot.x * i.size, i.pivot.y * i.size, i.pivot.z * i.size));
    transform->SetPivotEnable(true);
    transform->SetRotate({ i.rotation.x ,i.rotation.y ,i.rotation.z });

    auto mesh = GetComponent<MeshRenderer>();
    mesh->SetMaterial(i.material);
    mesh->SetRenderID(i.renderId);

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
