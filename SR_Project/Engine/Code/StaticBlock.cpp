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

StaticBlock::StaticBlock(ObjectManager* owner, ObjectType objType, StaticBlockType staticBlockType, StaticBlockDir staticBlockDir)
    : Object(owner, objType), Dir(staticBlockDir), Type(staticBlockType)
{
}

StaticBlock::~StaticBlock()
{
}

StaticBlock* StaticBlock::Create(ObjectManager* owner, ObjectType objType, StaticBlockType staticBlockType, StaticBlockDir staticBlockDir)
{
    StaticBlock* Instance = new StaticBlock(owner, objType, staticBlockType, staticBlockDir);

    if (FAILED(Instance->Ready_Object()))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"StaticBox Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT StaticBlock::Ready_Object()
{
    /////////////////////////////
    auto collision = AddComponent<CollisionComponent>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);
    collision->SetLayer(CollisionComponent::LAYER_DEFAULT);
    collision->SetMask(CollisionComponent::LAYER_PLAYER | CollisionComponent::LAYER_ENEMY);

    auto physics = AddComponent<PhysicsComponent>();
    ///////////////////////////////
    auto transform = AddComponent<TransformComponent>();
    transform->SetScale(1.f, 1.f, 1.f);
    
    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    renderer->SetMesh(L"Cube_Mesh");
    switch (Type)
    {
    case StaticBlockType::Dirt:
        renderer->SetMaterial(L"DirtBlock_Mtrl");
        break;
    case StaticBlockType::GrassDirt:
        renderer->SetMaterial(L"GrassBlock_Mtrl");
        break;
    case StaticBlockType::Wood:
        renderer->SetMaterial(L"WoodBlock_Mtrl");
        break;
    case StaticBlockType::WoodPlank:
        renderer->SetMaterial(L"WoodPlank_Mtrl");
        break;
    case StaticBlockType::Stone:
        renderer->SetMaterial(L"Stone_Mtrl");
        break;
    case StaticBlockType::CobbleStone:
        renderer->SetMaterial(L"CobbleStone_Mtrl");
        break;
    }
    
    switch (Dir)
    {
    case StaticBlockDir::BlockX:
        transform->SetRotate({ 0.f, 0.f, D3DXToRadian(90.f) });
        break;
    case StaticBlockDir::BlockY:
        transform->SetRotate({ 0.f, 0.f, 0.f });
        break;
    case StaticBlockDir::BlockZ:
        transform->SetRotate({ D3DXToRadian(-90.f), 0.f, 0.f });
        break;
    }

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