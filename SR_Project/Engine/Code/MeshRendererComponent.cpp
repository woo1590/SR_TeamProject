#include "EnginePCH.h"
#include "MeshRendererComponent.h"
#include "GraphicDevice.h"
#include "Object.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "RenderSystem.h"

//component
#include "TransformComponent.h"

//model
#include "Mesh.h"
#include "Material.h"

MeshRenderer::MeshRenderer(Object* owner, RENDER_ID renderId)
	:RendererComponent(owner,renderId)
{
}

MeshRenderer::~MeshRenderer()
{
}

MeshRenderer* MeshRenderer::Create(Object* owner, RENDER_ID renderId)
{
	MeshRenderer* Instance = new MeshRenderer(owner, renderId);
	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}
	
	return Instance;
}

void MeshRenderer::Late_Update(_float dt)
{
	RendererComponent::Late_Update(dt);
}

void MeshRenderer::Render()
{
	/*auto mesh = owner->GetComponent<MeshComponent>();
	auto material = owner->GetComponent<MaterialComponent>();
	auto dxMesh = owner->GetComponent<DxMeshComponent>();
	auto transform = owner->GetComponent<TransformComponent>();

	if (!transform) return;

	_matrix worldMat = transform->GetWorldMatrix();

	Device->SetTransform(D3DTS_WORLD, &worldMat);

	if (mesh)
	{
		if (material)
			material->Apply(0);
		mesh->Draw();
	}
	else if (dxMesh)
	{
		
		for (int i = 0; i < dxMesh->GetNumSubset(); ++i)
		{
			material->Apply(i);
			dxMesh->DrawSubset(i);
		}
	}*/

	auto transform = owner->GetComponent<TransformComponent>();
	_matrix worldMat = transform->GetWorldMatrix();
	Device->SetTransform(D3DTS_WORLD,&worldMat);

	mtrl->Apply();
	mesh->Draw();
}

void MeshRenderer::SetMesh(const std::wstring& key)
{
	mesh = EngineCore::GetInstance()->GetResourceManager()->GetMesh(key);
}

void MeshRenderer::SetMaterial(const std::wstring& key)
{
	mtrl = EngineCore::GetInstance()->GetResourceManager()->GetMaterial(key);
}

void MeshRenderer::Free()
{
	RendererComponent::Free();
}
