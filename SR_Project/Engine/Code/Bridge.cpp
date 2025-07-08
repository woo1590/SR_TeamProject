#include "EnginePCH.h"
#include "Bridge.h"
#include "UIRenderer.h"

//system
#include "EngineCore.h"
#include "ObjectManager.h"
#include "InputSystem.h"
#include "TimerManager.h"

//object
#include "Part.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

Bridge::Bridge(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col, int Count)
    : DynamicBlock(owner, objType, type, col, Rot, Count)
{
}

Bridge::~Bridge()
{
}

Bridge* Bridge::Create(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col, int Count)
{
    Bridge* Instance = new Bridge(owner, objType, type, col, Count);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"Bridge Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Bridge::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Object::Ready_Object();
    auto transform = AddComponent<TransformComponent>();

    Parts["IronParCage"] = Part::Create(owner, objType, _vec3(0.1f, 4.f, 0.1f), this, "IronCage_Mtrl");
    auto ironParTrans = Parts["IronParCage"]->GetComponent<TransformComponent>();

    Parts["IronCage0"] = Part::Create(owner, objType, _vec3(0.1f, 4.f, 0.1f), Parts["IronParCage"], "IronCage_Mtrl");
    auto ironTrans = Parts["IronCage0"]->GetComponent<TransformComponent>();

    ironParTrans->Translate(-0.5f, 3.f, 0.f);
    ironTrans->Translate(1.f, 0.f, 0.f);

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

    for (auto& part : Parts)
        owner->AddObject(objType, part.second);

    return S_OK;
}

void Bridge::Update(_float dt)
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
        Operate();

    Object::Update(dt);
}

void Bridge::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Bridge::SetMaterial(const std::string& mtrl, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<MeshRenderer>()->SetMaterial(mtrl);
}

void Bridge::SetPosition(_vec3 position, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetPosition(position);
}

void Bridge::SetRotation(_vec3 rotation, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetRotate(rotation);
}

void Bridge::Operate()
{
    
}

void Bridge::Free()
{
    Object::Free();
}