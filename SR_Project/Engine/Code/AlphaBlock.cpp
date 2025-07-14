#include "EnginePCH.h"
#include "AlphaBlock.h"
#include "UIRenderer.h"

//system
#include "EngineCore.h"
#include "ObjectManager.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

AlphaBlock::AlphaBlock(ObjectManager* owner, ObjectType objType, StaticBlockType type)
    : StaticBlock(owner, objType, type, sAEnd, sREnd, Alpha)
{
}

AlphaBlock::~AlphaBlock()
{
}

AlphaBlock* AlphaBlock::Create(ObjectManager* owner, ObjectType objType, StaticBlockType type)
{
    AlphaBlock* Instance = new AlphaBlock(owner, objType, type);

    if (FAILED(Instance->Ready_Object(owner, objType, type)))
    {
        Safe_Release(Instance);
        MessageBoxW(nullptr, L"AlphaBlock Created Failed", L"Fail", MB_OK);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT AlphaBlock::Ready_Object(ObjectManager* owner, ObjectType objType, StaticBlockType type)
{
    StaticBlock::Ready_Object();

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);
    renderer->SetMesh("Cube_Mesh");

    switch (type)
    {
    case StaticBlockType::Glass:
        renderer->SetMaterial("Glass_Mtrl");
        break;
    case StaticBlockType::Leaf:
        renderer->SetMaterial("Leaf_Mtrl");
        break;
    }

    return S_OK;
}

void AlphaBlock::Update(_float dt)
{
    Object::Update(dt);
}

void AlphaBlock::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void AlphaBlock::Free()
{
    Object::Free();
}