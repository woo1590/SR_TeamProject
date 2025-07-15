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

SpriteRenderer::SpriteRenderer(Object* owner, const std::string& name, _uint total, _float speed, _float size, _bool repeat)
	:RendererComponent(owner, RENDER_ID::Render_Alpha), spriteName(name), totalFrame(total), speed(speed), isRepeat(repeat), size(size)
{

}

SpriteRenderer::~SpriteRenderer()
{
}

SpriteRenderer* SpriteRenderer::Create(Object* owner, const std::string& name, _uint total, _float speed, _float size, _bool repeat)
{
	SpriteRenderer* Instance = new SpriteRenderer(owner, name, total, speed, size, repeat);
	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT SpriteRenderer::Ready_Component()
{
	quadMesh = QuadMesh::Create(size);
	shader = EngineCore::GetInstance()->GetResourceManager()->GetShader("SpriteShader");

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
			isSpriteEnd = true;
		}
	}
}

void SpriteRenderer::Render()
{
	if (!shader || !quadMesh) return;

	auto tex = EngineCore::GetInstance()->GetResourceManager()->GetTexture(spriteName + std::to_string(currFrame));
	shader->SetTexture("AlbedoMap", tex);

	auto transform = owner->GetComponent<TransformComponent>();
	auto cam = owner->GetScene()->GetCameraManager()->GetMainCamera();

	if (!cam)
		return;

	_matrix worldMat = transform->GetWorldMatrix();
	_matrix viewMat = cam->GetViewMatrix();

	//Billboard
	_matrix invRotY, invRotX;
	D3DXMatrixIdentity(&invRotY);
	D3DXMatrixIdentity(&invRotX);

	invRotY._11 = viewMat._11;
	invRotY._13 = viewMat._13;
	invRotY._31 = viewMat._31;
	invRotY._33 = viewMat._33;

	invRotX._22 = viewMat._22;
	invRotX._23 = viewMat._23;
	invRotX._32 = viewMat._32;
	invRotX._33 = viewMat._33;

	D3DXMatrixInverse(&invRotY, nullptr, &invRotY);
	D3DXMatrixInverse(&invRotX, nullptr, &invRotX);
	worldMat = invRotY * invRotX * worldMat;

	_matrix projMat = cam->GetProjMatrix();

	shader->Begin(0);

	shader->SetConstant("g_World", worldMat);
	shader->SetConstant("g_View", viewMat);
	shader->SetConstant("g_Proj", projMat);

	quadMesh->Draw();

	shader->End();
}

void SpriteRenderer::Free()
{
	Safe_Release(quadMesh);
}

