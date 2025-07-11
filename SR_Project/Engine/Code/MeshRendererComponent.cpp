#include "EnginePCH.h"
#include "MeshRendererComponent.h"
#include "GraphicDevice.h"
#include "Object.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "CameraManager.h"
#include "Scene.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"

//model
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

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
	//auto device = EngineCore::GetInstance()->GetRenderSystem()->GetDevice();
	//
	//// 💡 알파 블렌딩 상태 강제 설정
	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//
	//// 💡 텍스처 알파 사용 설정
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	auto transform = owner->GetComponent<TransformComponent>();
	auto cam = owner->GetScene()->GetCameraManager()->GetMainCamera();
	auto shader = mtrl->GetShader();

	_matrix worldMat = transform->GetWorldMatrix();
	_matrix viewMat = cam->GetViewMatrix();
	_matrix projMat = cam->GetProjMatrix();

	if (shader)
	{
		shader->Begin(0);
		shader->SetConstant("g_World", worldMat);
		shader->SetConstant("g_View", viewMat);
		shader->SetConstant("g_Proj", projMat);
	}

	mtrl->Apply();
	mesh->Draw();

	if(shader)
		shader->End();
}

void MeshRenderer::SetMesh(const std::string& key)
{
	mesh = EngineCore::GetInstance()->GetResourceManager()->GetMesh(key);
}

void MeshRenderer::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void MeshRenderer::SetMaterial(const std::string & key)
{
	mtrl = EngineCore::GetInstance()->GetResourceManager()->GetMaterial(key);
}

Material* MeshRenderer::GetMaterial()
{
	return mtrl;
}

void MeshRenderer::Free()
{
	RendererComponent::Free();
}
