#include "pch.h"
#include "TestBlock.h"
#include "UIRenderer.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"

TestBlock::TestBlock(ObjectManager* owner, ObjectType objType) : Object(owner, objType)
{
}

TestBlock::~TestBlock()
{
}

TestBlock* TestBlock::Create(ObjectManager* owner, ObjectType objType)
{
    TestBlock* Instance = new TestBlock(owner, objType);

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
    renderer->SetMesh(L"DirtBlock");
    renderer->SetMaterial(L"DirtBlock_Mtrl");

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