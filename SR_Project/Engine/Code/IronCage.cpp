#include "EnginePCH.h"
#include "IronCage.h"
#include "UIRenderer.h"

//system
#include "EngineCore.h"
#include "ObjectManager.h"
#include "InputSystem.h"
#include "TimerManager.h"
#include "SoundManager.h"

//object
#include "Part.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

IronCage::IronCage(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col, DynamicBlockRot rot, int Count)
    : DynamicBlock(owner, objType, type, col, rot, Count)
{
    AddRef();
}

IronCage::~IronCage()
{
}

IronCage* IronCage::Create(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col, DynamicBlockRot rot, int Count)
{
    IronCage* Instance = new IronCage(owner, objType, type, col, rot, Count);

    if (FAILED(Instance->Ready_Object(owner, objType, rot)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT IronCage::Ready_Object(ObjectManager* owner, ObjectType objType, DynamicBlockRot rot)
{
    if (Count < 1)
        return E_FAIL;

    Object::Ready_Object();
    auto transform = AddComponent<TransformComponent>();

    Parts["IronParCage"] = Part::Create(owner, objType, _vec3(0.1f, 4.f, 0.1f), this, "IronCage_Mtrl");
    auto ironParTrans = Parts["IronParCage"]->GetComponent<TransformComponent>();

    Parts["IronCage0"] = Part::Create(owner, objType, _vec3(0.1f, 4.f, 0.1f), Parts["IronParCage"], "IronCage_Mtrl");
    auto ironTrans = Parts["IronCage0"]->GetComponent<TransformComponent>();

    ironParTrans->Translate(-0.5f, 3.f, 0.f);
    ironTrans->Translate(1.f, 0.f, 0.f);

    ironParTrans->SetIsBlock();
    ironParTrans->SetPivotEnable(TRUE);
    ironParTrans->SetPivot(_vec3(0.5f, -3.f, 0.f));

    for (int i = 1; i < Count; ++i)
    {
        string fir = "IronCage_" + std::to_string(i) + "_1";
        Parts[fir] = Part::Create(owner, objType, _vec3(0.1f, 4.f, 0.1f), Parts["IronParCage"], "IronCage_Mtrl");
        auto first = Parts[fir]->GetComponent<TransformComponent>();

        string sec = "IronCage_" + std::to_string(i) + "_2";
        Parts[sec] = Part::Create(owner, objType, _vec3(0.1f, 4.f, 0.1f), Parts["IronParCage"], "IronCage_Mtrl");
        auto second = Parts[sec]->GetComponent<TransformComponent>();

        first->Translate(2.f * i, 0.f, 0.f);
        second->Translate(2.f * i + 1.f, 0.f, 0.f);
    }

    switch (rot)
    {
    case DynamicBlockRot::drZP:
        ironParTrans->SetRotate(0.f, D3DXToRadian(-90.f), 0.f);
        break;
    case DynamicBlockRot::drZM:
        ironParTrans->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
        break;
    case DynamicBlockRot::drXM:
        ironParTrans->SetRotate(0.f, D3DXToRadian(180.f), 0.f);
        break;
    }

    for (auto& part : Parts)
        owner->AddObject(ObjectType::Part, part.second);

    return S_OK;
}

void IronCage::Update(_float dt)
{
    if (First)
    {
        DynamicBlock::LoadLink();
        First = false;
    }

    bool allTriggered = !LinkedObject.empty();
    for (auto& Dst : LinkedObject)
    {
        if (!Dst || !static_cast<DynamicBlock*>(Dst)->GetTrigger())
        {
            allTriggered = false;
            break;
        }
    }
    Activate = allTriggered;

    if (Activate && !Trigger)
    {
        if (!playSound)
        {
            EngineCore::GetInstance()->GetSoundManager()->PlaySFX("CageOpen");
            playSound = true;
        }

        Operate(dt);
    }

    Object::Update(dt);
}

void IronCage::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void IronCage::SetMaterial(const std::string& mtrl, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<MeshRenderer>()->SetMaterial(mtrl);
}

void IronCage::SetScale(float scale)
{
    Scale = scale;
}

void IronCage::SetPosition(_vec3 position, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetPosition(position);
}

void IronCage::SetRotation(_vec3 rotation, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetRotate(rotation);
}

void IronCage::Operate(_float dt)
{
    auto ironParTrans = Parts["IronParCage"]->GetComponent<TransformComponent>();

    totTrans += Speed * dt;
    ironParTrans->Translate(0.f, -Speed * dt, 0.f);

    if (totTrans >= 7.9f) Trigger = true;
}

void IronCage::Free()
{
    DynamicBlock::Free();
}