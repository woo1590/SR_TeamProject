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
    Object* Instance(nullptr);

    switch (type)
    {
    case DynamicBlockType::LeverSwitch:
        if (dir != DynamicBlockDir::DBEnd)
            Instance = Lever::Create(owner, ObjectType::DynamicBlock, type, dir);
        break;
    case DynamicBlockType::BasicChest:
        Instance = Chest::Create(owner, ObjectType::DynamicBlock, type, dir);
        break;
    case DynamicBlockType::IronCages:
        if (dir == DynamicBlockDir::YP)
            Instance = IronCage::Create(owner, ObjectType::DynamicBlock, type, dir, Count);
        break;
    default:
        MessageBoxW(nullptr, L"Invalid DynamicBlockType", L"DynamicBlock::Create Error", MB_OK);
        break;
    }

    return Instance;
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
