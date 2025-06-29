#include "pch.h"
#include "TestBlock.h"
#include "UIRenderer.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"

TestBlock::TestBlock(ObjectManager* owner, ObjectType objType, BlockType blockType) : Object(owner, objType), m_eType(blockType)
{
}

TestBlock::~TestBlock()
{
}

TestBlock* TestBlock::Create(ObjectManager* owner, ObjectType objType, BlockType blockType)
{
    TestBlock* Instance = new TestBlock(owner, objType, blockType);

    if (FAILED(Instance->Ready_Object()))
    {
        Safe_Release(Instance);
        MSG_BOX("TestBlock Create Failed");
        Instance = nullptr;
    }

    return Instance;
}

HRESULT TestBlock::Ready_Object()
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetScale(1.f, 1.f, 1.f);

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    
    switch (m_eType)
    {
    case Dirt:
        renderer->SetMesh(L"DirtBlock");
        renderer->SetMaterial(L"DirtBlock_Mtrl");
        break;

    case GrassDirt:
        renderer->SetMesh(L"GrassBlock");
        renderer->SetMaterial(L"GrassBlock_Mtrl");
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