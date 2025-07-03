#include "EnginePCH.h"
#include "DynamicBlock.h"
#include "UIRenderer.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

DynamicBlock::DynamicBlock(ObjectManager* owner, ObjectType objType, DynamicBlockType dynamicBlockType, DynamicBlockDir dynamicBlockDir)
    : Object(owner, objType), Dir(dynamicBlockDir), Type(dynamicBlockType)
{
}

DynamicBlock::~DynamicBlock()
{
}

DynamicBlock* DynamicBlock::Create(ObjectManager* owner, ObjectType objType, DynamicBlockType dynamicBlockType, DynamicBlockDir dynamicBlockDir)
{
    DynamicBlock* Instance = new DynamicBlock(owner, objType, dynamicBlockType, dynamicBlockDir);

    if (FAILED(Instance->Ready_Object()))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"DynamicBlock Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT DynamicBlock::Ready_Object()
{
    auto collision = AddComponent<CollisionComponent>();
    auto transform = AddComponent<TransformComponent>();
    transform->SetScale(1.f, 1.f, 1.f);

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    switch (Type)
    {
    case DynamicBlockType::LeverSwitch:

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