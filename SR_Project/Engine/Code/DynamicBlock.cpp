#include "EnginePCH.h"
#include "DynamicBlock.h"

// Components
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

// Concrete blocks
#include "Lever.h"
#include "Chest.h"
#include "IronCage.h"

USING(Engine)

DynamicBlock::DynamicBlock(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockDir dir, int Count)
    : Object(owner, objType), Type(type), Dir(dir), Count(Count)
{
}

DynamicBlock::~DynamicBlock()
{
}

Object* DynamicBlock::Create(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockDir dir, int Count)
{
    Object* block = nullptr;

    switch (type)
    {
    case DynamicBlockType::LeverSwitch:
        block = Lever::Create(owner, ObjectType::DynamicBlock, type, dir);
        break;
    case DynamicBlockType::BasicChest:
        block = Chest::Create(owner, ObjectType::DynamicBlock, type, dir);
        break;
    case DynamicBlockType::IronCages:
        block = IronCage::Create(owner, ObjectType::DynamicBlock, type, dir, Count);
        break;
    default:
        MessageBoxW(nullptr, L"Invalid DynamicBlockType", L"DynamicBlock::Create Error", MB_OK);
        break;
    }

    return block;
}

HRESULT DynamicBlock::Ready_Object()
{
    auto collision = AddComponent<CollisionComponent>();
    auto transform = AddComponent<TransformComponent>();
    transform->SetScale(1.f, 1.f, 1.f);

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);

    return S_OK;
}

void DynamicBlock::Update(_float dt)
{
    Object::Update(dt);
}

void DynamicBlock::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void DynamicBlock::Free()
{
    Object::Free();
}
