#include "EnginePCH.h"
#include "Chest.h"
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

Chest::Chest(ObjectManager* owner, ObjectType objType, DynamicBlockType DynamicBlockType, DynamicBlockDir DynamicBlockDir)
    : DynamicBlock(owner, objType, DynamicBlockType, DynamicBlockDir, Count)
{
}

Chest::~Chest()
{
}

Chest* Chest::Create(ObjectManager* owner, ObjectType objType, DynamicBlockType DynamicBlockType, DynamicBlockDir DynamicBlockDir)
{
    Chest* Instance = new Chest(owner, objType, DynamicBlockType, DynamicBlockDir);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"Chest Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Chest::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Object::Ready_Object();
    auto transform = AddComponent<TransformComponent>();

    Parts["ChestDown"] = Part::Create(owner, objType, _vec3(1.f, 0.7f, 1.f), this, L"ChestDown_Mtrl");
    auto downTrans = Parts["ChestDown"]->GetComponent<TransformComponent>();

    Parts["ChestUp"] = Part::Create(owner, objType, _vec3(1.f, 0.3f, 1.f), Parts["ChestDown"], L"ChestUp_Mtrl");
    auto upTrans = Parts["ChestUp"]->GetComponent<TransformComponent>();

    Parts["ChestLock"] = Part::Create(owner, objType, _vec3(0.125f, 0.2f, 0.1f), Parts["ChestUp"], L"ChestLock_Mtrl");
    auto lockTrans = Parts["ChestLock"]->GetComponent<TransformComponent>();

    upTrans->SetIsBlock();

    Angle = 0.f;
    downTrans->Translate(0.f, -0.3f, 0.f);
    upTrans->SetPivotEnable(true);
    upTrans->SetPivot(_vec3(0.f, -0.3f, 1.f));
    upTrans->Translate(0.f, 1.f, 0.f);
    lockTrans->Translate(0.f, -0.2f, -1.f);

    switch (Dir)
    {
    case DynamicBlockDir::XP:
        downTrans->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
        break;
    case DynamicBlockDir::XM:
        downTrans->SetRotate(0.f, D3DXToRadian(270.f), 0.f);
        break;
    case DynamicBlockDir::ZM:
        downTrans->SetRotate(0.f, D3DXToRadian(180.f), 0.f);
        break;
    }

    for (auto& part : Parts)
        owner->AddObject(objType, part.second);

    return S_OK;
}

void Chest::Update(_float dt)
{
    if (!On)
        Operate();

    Object::Update(dt);
}

void Chest::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Chest::SetMaterial(const std::wstring& mtrl, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<MeshRenderer>()->SetMaterial(mtrl);
}

void Chest::SetScale(float scale)
{
    Scale = scale;
}

void Chest::SetPosition(_vec3 position, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetPosition(position);
}

void Chest::SetRotation(_vec3 rotation, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetRotate(rotation);
}

void Chest::Operate()
{
    auto upTrans = Parts["ChestUp"]->GetComponent<TransformComponent>();

    totAngle += rotSpeed;
    Angle += rotSpeed;
    upTrans->SetRotate(D3DXToRadian(Angle), 0.f, 0.f);

    if (totAngle >= 90.f) On = true;
}

void Chest::Free()
{
    Object::Free();
}