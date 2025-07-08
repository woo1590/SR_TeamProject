#include "EnginePCH.h"
#include "HalfBlock.h"
#include "UIRenderer.h"

//system
#include "EngineCore.h"
#include "ObjectManager.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

HalfBlock::HalfBlock(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
    : StaticBlock(owner, objType, type, axis, rot, usage)
{
}

HalfBlock::~HalfBlock()
{
}

HalfBlock* HalfBlock::Create(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
{
    HalfBlock* Instance = new HalfBlock(owner, objType, type, axis, rot, usage);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"HalfBlock Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT HalfBlock::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    StaticBlock::Ready_Object();

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    renderer->SetMesh("Cube_Mesh");

    auto transform = GetComponent<TransformComponent>();
    transform->SetScale(1.f, 0.5f, 1.f);
    transform->Translate(0.f, -0.5f, 0.f);

    switch (Type)
    {
    case StaticBlockType::WoodPlank:
        renderer->SetMaterial("WoodPlank_Half_Mtrl");
        break;
    case StaticBlockType::Stone:
        renderer->SetMaterial("Stone_Half_Mtrl");
        break;
    case StaticBlockType::CobbleStone:
        renderer->SetMaterial("CobbleStone_Half_Mtrl");
        break;
    case StaticBlockType::SmoothStone:
        renderer->SetMaterial("SmoothStone_Half_Mtrl");
        break;
    }

    return S_OK;
}


void HalfBlock::Update(_float dt)
{
    Object::Update(dt);
}

void HalfBlock::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void HalfBlock::Free()
{
    Object::Free();
}