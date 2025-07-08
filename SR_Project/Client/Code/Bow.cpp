#include "pch.h"
#include "Bow.h"

#include "ObjectManager.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"
#include "CollisionComponent.h"

Bow::Bow(ObjectManager* owner, ObjectType objType) : Item(owner, objType)
{
}
Bow::~Bow()
{
}
void Bow::Free()
{
    Item::Free();
}
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

    auto info = GetComponent<InfoComponent<ItemInfo>>();
    auto i = info->GetInfo();
    i.value = 10.f;
    info->SetInfo(i);

    SetMesh(L"Cube_Mesh");
    SetMaterial(L"bow_Mtrl");
    SetRenderId(renderId);

    PlayerBowInfo();

    ApplyComponents();

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

void Bow::PlayerBowInfo()
{
    SetScale(1.f);
    SetScaleRatio(_vec3(0.1f, 1.f, 1.f));
    SetPosition(_vec3(0.25, -1.2f, -0.2f));
    SetPivot(true, _vec3(-0.25f, 0.8f, -0.2f));
    SetRotation(_vec3(2.2f, 0.f, 0.f));
    SetOwnerObject(owner->GetFrontObject(ObjectType::Player));
    SetRenderId(Engine::RENDER_ID::Render_None);
}