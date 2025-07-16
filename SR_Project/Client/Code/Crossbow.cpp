#include "pch.h"
#include "Crossbow.h"

#include "ObjectManager.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"
#include "CollisionComponent.h"

Crossbow::Crossbow(ObjectManager* owner, ObjectType objType) : Item(owner, objType)
{
}

Crossbow::~Crossbow()
{
}

void Crossbow::Free()
{
    Item::Free();
}

Crossbow* Crossbow::Create(ObjectManager* owner, ObjectType objType)
{
    Crossbow* Instance = new Crossbow(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType))) {
        Safe_Release(Instance);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Crossbow::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    if (FAILED(Item::Ready_Object(owner, objType)))
        return E_FAIL;

    itemType = ItemType::CrossBow;
    auto info = GetComponent<InfoComponent<ItemInfo>>();
    auto i = info->GetInfo();
    i.value = 10.f;
    info->SetInfo(i);

    SetMesh("Cube_Mesh");
    SetMaterial("crossbow_Mtrl");
    SetRenderId(renderId);

    PlayerCrossbowInfo();

    ApplyComponents();

    return S_OK;
}

void Crossbow::Update(_float dt)
{
    Item::Update(dt);
}

void Crossbow::Late_Update(_float dt)
{
    Item::Late_Update(dt);
}

void Crossbow::PlayerCrossbowInfo()
{
    SetScale(1.f); // 다른 아이템이랑 동일, 문제없음
    SetScaleRatio(_vec3(1.f, 0.1f, 1.f)); // 스케일 1,1,1해도 동일, 문제없음
    SetPosition(_vec3(0.25, -2.f, 0.4f)); // 포지션 0,0,0해도 동일, 문제없음
    SetPivot(true, _vec3(-0.25f, 1.6f, -0.8f)); // 피벗 fale해도 동일, 문제없음
    SetRotation(_vec3(D3DXToRadian(90.f), D3DXToRadian(135.f), 0.f)); // 회전값 무슨값을 넣어도 적용안됨, 값 문제없음
    SetOwnerObject(owner->GetFrontObject(ObjectType::Player)); // 다른 아이템이랑 동일, 문제없음
    SetRenderId(Engine::RENDER_ID::Render_None); // 랜더값 Alpha해도 동일, 문제없음
}
