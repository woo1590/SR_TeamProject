#include "EnginePCH.h"
#include "TerrainRenderer.h"
#include "GraphicDevice.h"
#include "ResourceManager.h"
#include "EngineCore.h"
#include "Object.h"

#include "TerrainMesh.h"
#include "Material.h"

//component
#include "TransformComponent.h"

TerrainRenderer::TerrainRenderer(Object* owner, RENDER_ID renderId)
    :RendererComponent(owner,renderId),Device(GraphicDevice::GetInstance()->GetDevice())
{
    Device->AddRef();
}

TerrainRenderer::~TerrainRenderer()
{
}

TerrainRenderer* TerrainRenderer::Create(Object* owner, RENDER_ID renderId)
{
    TerrainRenderer* Instance = new TerrainRenderer(owner, renderId);

    if (FAILED(Instance->Ready_Component()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

void TerrainRenderer::Render()
{
    auto transform = owner->GetComponent<TransformComponent>();
    _matrix worldMat = transform->GetWorldMatrix();

    Device->SetTransform(D3DTS_WORLD, &worldMat);
    Mtrl->Apply();
    mesh->Draw();
}

void TerrainRenderer::SetMesh(const std::wstring& key)
{
    mesh = EngineCore::GetInstance()->GetResourceManager()->GetTerrain(key);
}

void TerrainRenderer::SetMaterial(const std::wstring& key)
{
    Mtrl = EngineCore::GetInstance()->GetResourceManager()->GetMaterial(key);
}

void TerrainRenderer::Free()
{
    Device->Release();
}
