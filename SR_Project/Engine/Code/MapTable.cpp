#include "EnginePCH.h"
#include "MapTable.h"
#include "UIRenderer.h"

//system
#include "EngineCore.h"
#include "ObjectManager.h"
#include "InputSystem.h"

//object
#include "Part.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

MapTable::MapTable(ObjectManager* owner, ObjectType objType, DynamicBlockType type)
    : DynamicBlock(owner, objType, type, Col, Rot, Count)
{
    AddRef();
}

MapTable::~MapTable()
{
}

MapTable* MapTable::Create(ObjectManager* owner, ObjectType objType, DynamicBlockType type)
{
    MapTable* Instance = new MapTable(owner, objType, type);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"MapTable Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT MapTable::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Object::Ready_Object();
    auto transform = AddComponent<TransformComponent>();

    Parts["MapTableUp"] = Part::Create(owner, objType, _vec3(4.f, 0.25f, 2.f), this, "MapTable_Up_Mtrl");
    auto upTrans = Parts["MapTableUp"]->GetComponent<TransformComponent>();

    Parts["MapTableMiddle"] = Part::Create(owner, objType, _vec3(3.75f, 0.6f, 1.75f), this, "MapTable_Middle_Mtrl");
    auto middleTrans = Parts["MapTableMiddle"]->GetComponent<TransformComponent>();

    Parts["MapTableDown"] = Part::Create(owner, objType, _vec3(4.f, 0.15f, 2.f), this, "MapTable_Down_Mtrl");
    auto downTrans = Parts["MapTableDown"]->GetComponent<TransformComponent>();

    upTrans->SetPosition(0.f, 0.7f, 0.f);
    middleTrans->SetPosition(0.f, -0.1f, 0.f);
    downTrans->SetPosition(0.f, -0.85f, 0.f);

    Parts["Candle4"] = Part::Create(owner, objType, _vec3(0.1f, 0.7f, 0.1f), this, "Candle4_Mtrl");
    auto can4Trans = Parts["Candle4"]->GetComponent<TransformComponent>();

    Parts["Candle1"] = Part::Create(owner, objType, _vec3(0.02f, 0.1f, 0.02f), Parts["Candle4"], "Candle1_Mtrl");
    auto can1Trans = Parts["Candle1"]->GetComponent<TransformComponent>();

    Parts["Candle2"] = Part::Create(owner, objType, _vec3(0.1f, 0.6f, 0.1f), Parts["Candle4"], "Candle2_Mtrl");
    auto can2Trans = Parts["Candle2"]->GetComponent<TransformComponent>();

    Parts["Candle3"] = Part::Create(owner, objType, _vec3(0.25f, 0.125f, 0.25f), Parts["Candle4"], "Candle3_Mtrl");
    auto can3Trans = Parts["Candle3"]->GetComponent<TransformComponent>();

    Parts["Candle5"] = Part::Create(owner, objType, _vec3(0.375f, 0.125f, 0.375f), Parts["Candle4"], "Candle5_Mtrl");
    auto can5Trans = Parts["Candle5"]->GetComponent<TransformComponent>();

    can1Trans->Translate(_vec3(0.f, 1.85f, 0.f));
    can2Trans->Translate(_vec3(0.f, 1.25f, 0.f));
    can3Trans->Translate(_vec3(0.f, 0.75f, 0.f));
    can4Trans->SetPosition(_vec3(-2.75f, 1.25f, 0.5f));

    Parts["Map"] = Part::Create(owner, objType, _vec3(3.f, 0.001f, 1.5f), this, "Map_Mtrl");
    auto mapTrans = Parts["Map"]->GetComponent<TransformComponent>();

    mapTrans->SetPosition(_vec3(0.f, 1.f, 0.f));
    mapTrans->SetRotate(0.f, D3DXToRadian(-10.f), 0.f);

    for (auto& part : Parts) owner->AddObject(ObjectType::Part, part.second);

    return S_OK;
}

void MapTable::Update(_float dt)
{
    Object::Update(dt);
}

void MapTable::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void MapTable::SetMaterial(const std::string& mtrl, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<MeshRenderer>()->SetMaterial(mtrl);
}

void MapTable::SetScale(float scale)
{
    Scale = scale;
}

void MapTable::SetPosition(_vec3 position, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetPosition(position);
}

void MapTable::SetRotation(_vec3 rotation, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetRotate(rotation);
}

void MapTable::Free()
{
    DynamicBlock::Free();
}