#include "EnginePCH.h"
#include "StairBlock.h"
#include "UIRenderer.h"

//system
#include "EngineCore.h"
#include "ObjectManager.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

//object
#include "Part.h"

StairBlock::StairBlock(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
    : StaticBlock(owner, objType, type, axis, rot, usage)
{
}

StairBlock::~StairBlock()
{
}

StairBlock* StairBlock::Create(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
{
    StairBlock* Instance = new StairBlock(owner, objType, type, axis, rot, usage);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"StairBlock Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT StairBlock::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    StaticBlock::Ready_Object();

    Parts["Bottom"] = Part::Create(owner, objType, _vec3(1.f, 0.5f, 1.f), this, L"WoodPlank_Half_Mtrl");
    auto bottom = Parts["Bottom"]->GetComponent<TransformComponent>();

    Parts["Up"] = Part::Create(owner, objType, _vec3(1.f, 0.5f, 0.5f), Parts["Bottom"], L"WoodPlank_Half_Mtrl");
    auto up = Parts["Up"]->GetComponent<TransformComponent>();

    bottom->SetIsBlock();
    bottom->Translate(0.f, -0.5f, 0.f);
    up->Translate(0.f, 1.f, -0.5f);

    switch (Type)
    {
    case StaticBlockType::Stone:
        SetMaterial(L"Stone_Half_Mtrl", "Bottom");
        SetMaterial(L"Stone_Half_Mtrl", "Up");
        break;
    case StaticBlockType::CobbleStone:
        SetMaterial(L"CobbleStone_Half_Mtrl", "Bottom");
        SetMaterial(L"CobbleStone_Half_Mtrl", "Up");
        break;
    case StaticBlockType::SmoothStone:
        SetMaterial(L"SmoothStone_Half_Mtrl", "Bottom");
        SetMaterial(L"SmoothStone_Half_Mtrl", "Up");
        break;
    }

    switch (Rot)
    {
    case StaticBlockRot::sZM:
        bottom->SetRotate(0.f, D3DXToRadian(180.f), 0.f);
        break;
    case StaticBlockRot::sXP:
        bottom->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
        break;
    case StaticBlockRot::sXM:
        bottom->SetRotate(0.f, D3DXToRadian(-90.f), 0.f);
        break;
    }

    for (auto& part : Parts)
        owner->AddObject(objType, part.second);

    return S_OK;
}


void StairBlock::Update(_float dt)
{
    Object::Update(dt);
}

void StairBlock::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void StairBlock::SetMaterial(const std::wstring& mtrl, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<MeshRenderer>()->SetMaterial(mtrl);
}

void StairBlock::SetPosition(_vec3 position, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetPosition(position);
}

void StairBlock::SetRotation(_vec3 rotation, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetRotate(rotation);
}

void StairBlock::Free()
{
    Object::Free();
}