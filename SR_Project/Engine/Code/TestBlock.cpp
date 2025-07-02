#include "EnginePCH.h"
#include "TestBlock.h"
#include "UIRenderer.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

TestBlock::TestBlock(ObjectManager* owner, ObjectType objType, BlockType blockType, BlockDir blockDir)
    : Object(owner, objType), m_eDir(blockDir), m_eType(blockType)
{
}

TestBlock::~TestBlock()
{
}

TestBlock* TestBlock::Create(ObjectManager* owner, ObjectType objType, BlockType blockType, BlockDir blockDir)
{
    TestBlock* Instance = new TestBlock(owner, objType, blockType, blockDir);

    if (FAILED(Instance->Ready_Object()))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"TestBox Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT TestBlock::Ready_Object()
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetScale(1.f, 1.f, 1.f);

    auto collision = AddComponent<CollisionComponent>();
    collision->SetLayer(CollisionComponent::LAYER_DEFAULT);
    collision->SetMask(CollisionComponent::LAYER_PLAYER | CollisionComponent::LAYER_ENEMY);
    
    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    switch (m_eType)
    {
    case Dirt:
        renderer->SetMesh(L"Cube_Mesh");
        renderer->SetMaterial(L"DirtBlock_Mtrl");
        break;
    case GrassDirt:
        renderer->SetMesh(L"GrassBlock");
        renderer->SetMaterial(L"GrassBlock_Mtrl");
        break;
    case Wood:
        renderer->SetMesh(L"WoodBlock");
        renderer->SetMaterial(L"WoodBlock_Mtrl");
        break;
    case WoodPlank:
        renderer->SetMesh(L"WoodPlank");
        renderer->SetMaterial(L"WoodPlank_Mtrl");
        break;
    }
    
    switch (m_eDir)
    {
    case BlockDir::BlockX:
        transform->SetRotate({ 0.f, 0.f, D3DXToRadian(90.f) });
        break;
    case BlockDir::BlockY:
        transform->SetRotate({ 0.f, 0.f, 0.f });
        break;
    case BlockDir::BlockZ:
        transform->SetRotate({ D3DXToRadian(-90.f), 0.f, 0.f });
        break;
    }

    return S_OK;
}

void TestBlock::Update(_float dt)
{
    Object::Update(dt);
}

void TestBlock::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void TestBlock::Free()
{
    Object::Free();
}