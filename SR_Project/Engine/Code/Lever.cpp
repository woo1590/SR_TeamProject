#include "EnginePCH.h"
#include "Lever.h"
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

int Lever::totID = 0;

Lever::Lever(ObjectManager* owner, ObjectType objType, DynamicBlockType DynamicBlockType, DynamicBlockAxis DynamicBlockAxis)
    : DynamicBlock(owner, objType, DynamicBlockType, DynamicBlockAxis, Count)
{
    ID = totID++;
}

Lever::~Lever()
{
}

Lever* Lever::Create(ObjectManager* owner, ObjectType objType, DynamicBlockType DynamicBlockType, DynamicBlockAxis DynamicBlockAxis)
{
    Lever* Instance = new Lever(owner, objType, DynamicBlockType, DynamicBlockAxis);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"Lever Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Lever::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Object::Ready_Object();
    auto transform = AddComponent<TransformComponent>();

    Parts["Base"] = Part::Create(owner, objType, _vec3(0.3, 0.4f, 0.3f), this, L"Stone_Mtrl");
    auto baseTransform = Parts["Base"]->GetComponent<TransformComponent>();

    Parts["Handle"] = Part::Create(owner, objType, _vec3(0.5f, 0.1f, 0.1f), Parts["Base"], L"Lever_Mtrl");
    auto handleTransform = Parts["Handle"]->GetComponent<TransformComponent>();

    handleTransform->SetIsBlock();

    switch (Axis)
    {
    case DynamicBlockAxis::dXP:
        Angle = 40.f;
        baseTransform->SetPosition(-1.f, 0.f, 0.f);
        handleTransform->SetPivotEnable(true);
        handleTransform->SetPivot(_vec3(-0.5f, 0.f, 0.f));
        handleTransform->Translate(0.75f, 0.f, 0.f);
        handleTransform->SetRotate(0.f, 0.f, D3DXToRadian(Angle));
        break;
    case DynamicBlockAxis::dXM:
        Angle = -40.f;
        baseTransform->SetPosition(1.f, 0.f, 0.f);
        handleTransform->SetPivotEnable(true);
        handleTransform->SetPivot(_vec3(0.5f, 0.f, 0.f));
        handleTransform->Translate(-0.75f, 0.f, 0.f);
        handleTransform->SetRotate(0.f, 0.f, D3DXToRadian(Angle));
        break;

    case DynamicBlockAxis::dZP:
        Angle = -40.f;
        baseTransform->SetPosition(0.f, 0.f, -1.f);
        handleTransform->SetPivotEnable(true);
        handleTransform->SetPivot(_vec3(-0.5f, 0.f, 0.f));
        handleTransform->Translate(0.5f, 0.f, 0.25f);
        handleTransform->SetRotate(D3DXToRadian(Angle), D3DXToRadian(-90.f), 0.f);
        break;
    case DynamicBlockAxis::dZM:
        Angle = 40.f;
        baseTransform->SetPosition(0.f, 0.f, 1.f);
        handleTransform->SetPivotEnable(true);
        handleTransform->SetPivot(_vec3(-0.5f, 0.f, 0.f));
        handleTransform->Translate(0.5f, 0.f, -0.25f);
        handleTransform->SetRotate(D3DXToRadian(Angle), D3DXToRadian(90.f), 0.f);
        break;
    }

    for (auto& part : Parts)
        owner->AddObject(objType, part.second);

    return S_OK;
}

void Lever::Update(_float dt)
{
    if (Activate && !Trigger)
        Operate();

    Object::Update(dt);
}

void Lever::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Lever::SetMaterial(const std::wstring& mtrl, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<MeshRenderer>()->SetMaterial(mtrl);
}

void Lever::SetScale(float scale)
{
    Scale = scale;
}

void Lever::SetPosition(_vec3 position, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetPosition(position);
}

void Lever::SetRotation(_vec3 rotation, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetRotate(rotation);
}

void Lever::Operate()
{
    auto handleTransform = Parts["Handle"]->GetComponent<TransformComponent>();

    switch (Axis)
    {
    case DynamicBlockAxis::dXP: case DynamicBlockAxis::dZM:
        totAngle += rotSpeed;
        Angle -= rotSpeed;
        break;
    case DynamicBlockAxis::dZP: case DynamicBlockAxis::dXM:
        totAngle += rotSpeed;
        Angle += rotSpeed;
        break;
    }

    switch (Axis)
    {
    case DynamicBlockAxis::dXP: case DynamicBlockAxis::dXM:
        handleTransform->SetRotate(0.f, 0.f, D3DXToRadian(Angle));
        break;
    case DynamicBlockAxis::dZP:
        handleTransform->SetRotate(D3DXToRadian(Angle), D3DXToRadian(-90.f), 0.f);
        break;
    case DynamicBlockAxis::dZM:
        handleTransform->SetRotate(D3DXToRadian(Angle), D3DXToRadian(90.f), 0.f);
        break;
    }

    if (totAngle >= 90.f) Trigger = true;
}

void Lever::Free()
{
    Object::Free();
}