#include "EnginePCH.h"
#include "BasicBlock.h"
#include "UIRenderer.h"

//system
#include "EngineCore.h"
#include "ObjectManager.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

BasicBlock::BasicBlock(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
    : StaticBlock(owner, objType, type, axis, rot, usage)
{
}

BasicBlock::~BasicBlock()
{
}

BasicBlock* BasicBlock::Create(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
{
    BasicBlock* Instance = new BasicBlock(owner, objType, type, axis, rot, usage);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"BasicBlock Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT BasicBlock::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    StaticBlock::Ready_Object();

    /*auto transform = GetComponent<TransformComponent>();
    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    renderer->SetMesh("Cube_Mesh");

    switch (Type)
    {
    case StaticBlockType::Dirt:
        renderer->SetMaterial("DirtBlock_Mtrl");
        break;
    case StaticBlockType::GrassDirt:
        renderer->SetMaterial("GrassBlock_Mtrl");
        break;
    case StaticBlockType::Wood:
        renderer->SetMaterial("WoodBlock_Mtrl");
        break;
    case StaticBlockType::WoodPlank:
        renderer->SetMaterial("WoodPlank_Mtrl");
        break;
    case StaticBlockType::Stone:
        renderer->SetMaterial("Stone_Mtrl");
        break;
    case StaticBlockType::CobbleStone:
        renderer->SetMaterial("CobbleStone_Mtrl");
        break;
    case StaticBlockType::SmoothStone:
        renderer->SetMaterial("SmoothStone_Mtrl");
        break;
    case StaticBlockType::StoneBrick:
        renderer->SetMaterial("StoneBrick_Mtrl");
        break;
    case StaticBlockType::MossyStoneBrick:
        renderer->SetMaterial("MossyStoneBrick_Mtrl");
        break;
    }

    switch (Axis)
    {
    case StaticBlockAxis::sAX:
        transform->SetRotate({ 0.f, 0.f, D3DXToRadian(90.f) });
        break;
    case StaticBlockAxis::sAY:
        transform->SetRotate({ 0.f, 0.f, 0.f });
        break;
    case StaticBlockAxis::sAZ:
        transform->SetRotate({ D3DXToRadian(-90.f), 0.f, 0.f });
        break;
    }*/

    return S_OK;
}

void BasicBlock::Update(_float dt)
{
    Object::Update(dt);
}

void BasicBlock::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void BasicBlock::Free()
{
    Object::Free();
}