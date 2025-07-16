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
    AddRef();
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
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Bridge::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Object::Ready_Object();
    auto transform = AddComponent<TransformComponent>();

    Parts["BriPar"] = Part::Create(owner, objType, _vec3(1.f, 1.f, 1.f), this, "WoodPlank_Mtrl");
    auto BriPar = Parts["BriPar"]->GetComponent<TransformComponent>();

    for (int i = 1; i <= 4; ++i)
    {
        string Bri = "Bri" + std::to_string(i);
        Parts[Bri] = Part::Create(owner, objType, _vec3(1.f, 1.f, 1.f), Parts["BriPar"], "WoodPlank_Mtrl");
        auto BriTrans = Parts[Bri]->GetComponent<TransformComponent>();

        BriTrans->Translate(2.f * i, 0.f, 0.f);
    }

    for (int i = 1; i <= 4; ++i)
    {
        for (int j = 0; j <= 4; ++j)
        {
            string Bri = "Bricount" + std::to_string(i) + "_" + std::to_string(j);
            Parts[Bri] = Part::Create(owner, objType, _vec3(1.f, 0.5f, 1.f), Parts["BriPar"], "WoodPlank_Mtrl");
            auto briSon = Parts[Bri]->GetComponent<TransformComponent>();
            briSon->Translate(2.f * j, 0.5f, -2.f * i);
        }
    }

    Parts["Stick1"] = Part::Create(owner, objType, _vec3(0.5f, 4.f, 0.5f), this, "WoodBlock_Mtrl");
    auto st1Trans = Parts["Stick1"]->GetComponent<TransformComponent>();

    Parts["Stick2"] = Part::Create(owner, objType, _vec3(0.5f, 4.f, 0.5f), this, "WoodBlock_Mtrl");
    auto st2Trans = Parts["Stick2"]->GetComponent<TransformComponent>();

    st1Trans->Translate(0.f, 5.f, 2.f);
    st2Trans->Translate(8.f, 5.f, 2.f);

    Parts["Stick3"] = Part::Create(owner, objType, _vec3(0.25f, 8.f, 0.25f), this, "WoodBlock_Mtrl");
    auto st3Trans = Parts["Stick3"]->GetComponent<TransformComponent>();

    Parts["Stick4"] = Part::Create(owner, objType, _vec3(0.25f, 8.f, 0.25f), Parts["Stick3"], "WoodBlock_Mtrl");
    auto st4Trans = Parts["Stick4"]->GetComponent<TransformComponent>();
    
    st3Trans->SetRotate(D3DXToRadian(90.f), 0.f, 0.f);
    st3Trans->Translate(1.5f, 8.f, 0.f);
    st4Trans->Translate(5.5f, 0.f, 0.f);

    Parts["Stick5"] = Part::Create(owner, objType, _vec3(4.f, 0.15f, 0.15f), this, "WoodPlank_Mtrl");
    auto st5Trans = Parts["Stick5"]->GetComponent<TransformComponent>();

    st5Trans->Translate(4.f, 8.f, 2.f);

    BriPar->SetIsBlock();
    BriPar->SetPivotEnable(TRUE);
    BriPar->SetPivot(_vec3(0.f, 0.f, 0.f));

    for (auto& part : Parts)
        owner->AddObject(ObjectType::Part, part.second);

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
    DynamicBlock::Free();
}