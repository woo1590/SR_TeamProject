#include "EnginePCH.h"
#include "SpriteRenderer.h"
#include "EngineCore.h"
#include "QuadMesh.h"
#include "Material.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "CameraManager.h"
#include "Object.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"

SpriteRenderer::SpriteRenderer(Object* owner, RENDER_ID id, const std::string& name, _uint total, _bool repeat)
	:RendererComponent(owner,id),spriteName(name), totalFrame(total), isRepeat(repeat)
{

}

SpriteRenderer::~SpriteRenderer()
{
}

SpriteRenderer* SpriteRenderer::Create(Object* owner, RENDER_ID id, const std::string& name, _uint total, _bool repeat)
{
	SpriteRenderer* Instance = new SpriteRenderer(owner, id, name, total, repeat);
	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT SpriteRenderer::Ready_Component()
{
	

	return S_OK;
}

void SpriteRenderer::Update(_float dt)
{
	if (currFrame < totalFrame)
	{
		timer += dt * speed;
	}

	if (timer >= 1.f)
	{
		timer = 0.f;
		currFrame++;
	}

	if (currFrame >= totalFrame)
	{
		if (isRepeat)
		{
			currFrame = 0;
		}
		else
		{
			currFrame--;
		}
	}
}

void SpriteRenderer::Render()
{
	if (!mtrl || !quadMesh) return;

	auto tex = EngineCore::GetInstance()->GetResourceManager()->GetTexture(spriteName + std::to_string(currFrame));
	mtrl->SetTexture("AlbedoMap", tex);

	auto shader = mtrl->GetShader();
	auto transform = owner->GetComponent<TransformComponent>();
	auto cam = owner->GetScene()->GetCameraManager()->GetMainCamera();

	if (!cam)
		return;

	_matrix worldMat = transform->GetWorldMatrix();
	_matrix viewMat = cam->GetViewMatrix();
	_matrix projMat = cam->GetProjMatrix();

	shader->Begin(0);

	shader->SetConstant("g_World", worldMat);
	shader->SetConstant("g_View", viewMat);
	shader->SetConstant("g_Proj", projMat);

	mtrl->Apply();
	quadMesh->Draw();

	shader->End();
}

void SpriteRenderer::SetMaterial(std::string& key)
{
	mtrl = EngineCore::GetInstance()->GetResourceManager()->GetMaterial(key);
}

void SpriteRenderer::SetMesh(const std::string& key)
{
	quadMesh = EngineCore::GetInstance()->GetResourceManager()->GetMesh(key);
	quadMesh->AddRef();
}

void SpriteRenderer::Free()
{
	Safe_Release(quadMesh);
	Safe_Release(mtrl);
}

