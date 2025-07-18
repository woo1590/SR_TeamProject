#include "pch.h"
#include "FireBlock.h"
#include "Material.h"
#include "Shader.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

FireBlock::FireBlock(ObjectManager* owner, ObjectType objType)
	:Object(owner,objType)
{
}

FireBlock::~FireBlock()
{
}

FireBlock* FireBlock::Create(ObjectManager* owner, ObjectType objType)
{
	FireBlock* Instance = new FireBlock(owner, objType);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT FireBlock::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();

	auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMesh("Cube_Mesh");
	renderer->SetMaterial("FireBlock_Mtrl");

	auto mtrl = renderer->GetMaterial();
	mtrl->SetVec3("uvScale", _vec3(1.f / 16.f, 1.f, 0.f));
	mtrl->SetVec3("uvOffset", _vec3(0.f, 0.f, 0.f));

	return S_OK;
}

void FireBlock::Update(_float dt)
{
	Object::Update(dt);

	if (timer >= 1.f)
	{
		timer = 0.f;
		currFrame++;

		if (currFrame >= totalFrame)
			currFrame = 0;

		auto mtrl = GetComponent<MeshRenderer>()->GetMaterial();
		mtrl->SetVec3("uvOffset", _vec3(currFrame * 1.f / 16.f, 0.f, 0.f));
	}

	{
		if (deadTime)
			deadTimer += dt;

		if (deadTimer >= deadTime)
			SetDead();
	}

	timer += dt * speed;
}

void FireBlock::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void FireBlock::SetColor(_vec3 color)
{
	this->color = color;
	auto mtrl = GetComponent<MeshRenderer>()->GetMaterial();
	mtrl->SetVec3("color",color);
}

void FireBlock::Free()
{
}
