#include "EnginePCH.h"
#include "StaticBlock.h"
#include "UIRenderer.h"
#include "Scene.h"
#include "CollisionSystem.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"

//object
#include "BasicBlock.h"
#include "HalfBlock.h"
#include "StairBlock.h"
#include "AlphaBlock.h"

StaticBlock::StaticBlock(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
    : Object(owner, objType), Axis(axis), Type(type), Rot(rot), Usage(usage)
{
}

StaticBlock::~StaticBlock()
{
}

Object* StaticBlock::Create(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
{
    Object* Instance(nullptr);

    switch (usage)
    {
    case StaticBlockUsage::Basic:
        Instance = BasicBlock::Create(owner, ObjectType::StaticBlock, type, axis, rot, usage);//
        break;
    case StaticBlockUsage::Half:
        Instance = HalfBlock::Create(owner, ObjectType::StaticBlock, type, axis, rot, usage);
        break;
    case StaticBlockUsage::Stair:
        Instance = StairBlock::Create(owner, ObjectType::StaticBlock, type, axis, rot, usage);
        break;
    case StaticBlockUsage::Alpha:
        Instance = AlphaBlock::Create(owner, ObjectType::StaticBlock, type, axis, rot, usage);
        break;
    default:
        MessageBoxW(nullptr, L"Invalid StaticBlockUsage", L"Error", MB_OK);
        break;
    }

    return Instance;
}

HRESULT StaticBlock::Ready_Object()
{
    auto collision = AddComponent<CollisionComponent>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);
    collision->SetLayer(CollisionComponent::LAYER_DEFAULT);
    collision->SetMask(CollisionComponent::LAYER_PLAYER | CollisionComponent::LAYER_ENEMY);
    auto physics = AddComponent<PhysicsComponent>();
    auto transform = AddComponent<TransformComponent>();
    transform->SetScale(1.f, 1.f, 1.f);
    
    return S_OK;
}

void StaticBlock::Update(_float dt)
{
    Object::Update(dt);
}

void StaticBlock::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void StaticBlock::Free()
{
    Object::Free();
}