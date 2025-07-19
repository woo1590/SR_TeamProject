#include "EnginePCH.h"
#include "DynamicBlock.h"
#include "ObjectManager.h"

// Components
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

// Concrete blocks
#include "Lever.h"
#include "Chest.h"
#include "IronCage.h"
#include "Bridge.h"

USING(Engine)

DynamicBlock::DynamicBlock(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col, DynamicBlockRot rot, int Count)
    : Object(owner, objType), Type(type), Col(col), Rot(rot), Count(Count)
{
}

DynamicBlock::~DynamicBlock()
{
}

Object* DynamicBlock::Create(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col, DynamicBlockRot rot, int Count)
{
    Object* Instance(nullptr);

    switch (type)
    {
    case DynamicBlockType::LeverSwitch:
        if (col != DynamicBlockCol::dAEnd && col != DynamicBlockCol::dYP)
            Instance = Lever::Create(owner, ObjectType::DynamicBlock, type, col);
        break;
    case DynamicBlockType::BasicChest:
        Instance = Chest::Create(owner, ObjectType::DynamicBlock, type, rot);
        break;
    case DynamicBlockType::IronCages:
        if (col == DynamicBlockCol::dYP)
            Instance = IronCage::Create(owner, ObjectType::DynamicBlock, type, col, rot, Count);
        break;
    default:
        MessageBoxW(nullptr, L"Invalid DynamicBlockType", L"Error", MB_OK);
        break;
    }

    return Instance;
}

HRESULT DynamicBlock::Ready_Object()
{
    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<AABBCollider>();
    collision->SetLayer(LAYER_DEFAULT);
    collision->SetMask(LAYER_PLAYER | LAYER_ENEMY);
    auto transform = AddComponent<TransformComponent>();
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

void DynamicBlock::LoadLink()
{
    for (auto& Dst : owner->GetObjectList(ObjectType::DynamicBlock))
    {
        if (find(LinkedID.begin(), LinkedID.end(), static_cast<DynamicBlock*>(Dst)->ID) != LinkedID.end())
            LinkedObject.push_back(Dst);
    }
}

void DynamicBlock::Free()
{
    Object::Free();
}
