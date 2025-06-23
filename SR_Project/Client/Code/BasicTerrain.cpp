#include "pch.h"
#include "BasicTerrain.h"

//component
#include "TransformComponent.h"
#include "TerrainRenderer.h"

BasicTerrain::BasicTerrain(ObjectManager* owner, ObjectType objType)
    :Object(owner,objType)
{
}

BasicTerrain::~BasicTerrain()
{
}

BasicTerrain* BasicTerrain::Create(ObjectManager* owner, ObjectType objType)
{
    BasicTerrain* Instance = new BasicTerrain(owner, objType);

    if (FAILED(Instance->Ready_Object()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT BasicTerrain::Ready_Object()
{
    Object::Ready_Object();

    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(0.f, 0.f, 0.f);
    transform->SetScale(1.f, 1.f, 1.f);

    auto renderer = AddComponent<TerrainRenderer>(RENDER_NONALPHA);
    renderer->SetMesh(L"Basic_Terrain");
    renderer->SetMaterial(L"Basic_Terrain_Material");

    return S_OK;
}

void BasicTerrain::Update(_float dt)
{
    Object::Update(dt);
}

void BasicTerrain::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void BasicTerrain::Free()
{
    Object::Free();
}
