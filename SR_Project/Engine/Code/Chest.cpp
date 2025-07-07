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

Chest::Chest(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockRot rot)
    : DynamicBlock(owner, objType, type, Col, rot, Count)
{
}

Chest::~Chest()
{
}

Chest* Chest::Create(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockRot rot)
{
    Chest* Instance = new Chest(owner, objType, type, rot);

    if (FAILED(Instance->Ready_Object(owner, objType, rot)))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"Chest Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Chest::Ready_Object(ObjectManager* owner, ObjectType objType, DynamicBlockRot rot)
{
    Object::Ready_Object();
    auto transform = AddComponent<TransformComponent>();

    Parts["ChestDown"] = Part::Create(owner, objType, _vec3(1.f, 0.7f, 1.f), this, L"ChestDown_Mtrl");
    auto downTrans = Parts["ChestDown"]->GetComponent<TransformComponent>();

    Parts["ChestUp"] = Part::Create(owner, objType, _vec3(1.f, 0.3f, 1.f), Parts["ChestDown"], L"ChestUp_Mtrl");
    auto upTrans = Parts["ChestUp"]->GetComponent<TransformComponent>();

    Parts["ChestLock"] = Part::Create(owner, objType, _vec3(0.2f, 0.25f, 0.1f), Parts["ChestUp"], L"ChestLock_Mtrl");
    auto lockTrans = Parts["ChestLock"]->GetComponent<TransformComponent>();

    PartScales["ChestDown"] = _vec3(1.f, 0.7f, 1.f);
    PartScales["ChestUp"] = _vec3(1.f, 0.3f, 1.f);
    PartScales["ChestLock"] = _vec3(0.2f, 0.25f, 0.1f);

    upTrans->SetIsBlock();

    Angle = 0.f;
    downTrans->Translate(0.f, -0.3f, 0.f);
    upTrans->SetPivotEnable(true);
    upTrans->SetPivot(_vec3(0.f, -0.3f, 1.f));
    upTrans->Translate(0.f, 1.f, 0.f);
    lockTrans->Translate(0.f, -0.2f, -1.f);

    switch (rot)
    {
    case DynamicBlockRot::drXP:
        downTrans->SetRotate(0.f, D3DXToRadian(-90.f), 0.f);
        break;
    case DynamicBlockRot::drXM:
        downTrans->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
        break;
    case DynamicBlockRot::drZP:
        downTrans->SetRotate(0.f, D3DXToRadian(180.f), 0.f);
        break;
    }

    for (auto& part : Parts)
        owner->AddObject(objType, part.second);

    return S_OK;
}

void Chest::Update(_float dt)
{
    if (First)
        Generate(dt);

    if (Activate && !Trigger)
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

void Chest::Generate(_float dt)
{
    firstTime += dt;
    float time = min(firstTime / 0.75f, 1.f);
    float t = EaseOutBack(time);

    for (auto& [name, part] : Parts)
    {
        auto transform = part->GetComponent<TransformComponent>();
        _vec3 maxScale = PartScales[name];
        _vec3 newScale = _vec3(maxScale.x * t, maxScale.y * t, maxScale.z * t);
        transform->SetScale(newScale);
    }

    if (time >= 1.f)
        First = false;
}

float Chest::EaseOutBack(float t)
{
    float c1 = 1.70158f;
    float c3 = c1 + 1.f;

    return 1.f + c3 * pow(t - 1, 3) + c1 * pow(t - 1, 2);
}

void Chest::Operate()
{
    auto upTrans = Parts["ChestUp"]->GetComponent<TransformComponent>();

    totAngle += rotSpeed;
    Angle += rotSpeed;
    upTrans->SetRotate(D3DXToRadian(Angle), 0.f, 0.f);

    if (totAngle >= 90.f) Trigger = true;
}

void Chest::Free()
{
    Object::Free();
}