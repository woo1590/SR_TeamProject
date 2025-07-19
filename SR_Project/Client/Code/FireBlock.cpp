#include "pch.h"
#include "FireBlock.h"
#include "Material.h"
#include "Shader.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"
#include "Scene.h"
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "InfoComponent.h"

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

	auto collision = AddComponent<CollisionComponent>();
	GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
	collision->SetLayer(LAYER_PROJECTILE);
	collision->SetMask(LAYER_PLAYER);
	collision->SetCollisionEnter([this](Object* other) {this->OnCollisionStay(other); });
	collision->SetSize(_vec3(1.f, 1.f, 1.f));

	auto physics = AddComponent<PhysicsComponent>();
	GetScene()->GetPhysicsStstem()->RegisterBody(physics);
	physics->SetKinematic(true);
	physics->SetMass(1.f);

	auto mtrl = renderer->GetMaterial();
	mtrl->SetVec3("uvScale", _vec3(1.f / 16.f, 1.f, 0.f));
	mtrl->SetVec3("uvOffset", _vec3(0.f, 0.f, 0.f));
	SetColor(color);
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
		{
			deadTimer += dt;

			if (deadTimer >= deadTime)
				SetDead();
		}
		else
		{
			deadTimer += dt;
			if (deadTimer >= 5.f)
				SetActive(false);
		}
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

void FireBlock::SetActive(_bool Active)
{
	auto renderer = GetComponent<MeshRenderer>();
	auto collision = GetComponent<CollisionComponent>();
	if (Active)
	{
		renderer->SetRenderID(RENDER_ID::Render_Alpha);
		collision->SetSize(_vec3(2.f, 2.f, 2.f));
	}
	else
	{
		renderer->SetRenderID(RENDER_ID::Render_None);
		collision->SetSize(_vec3(0.f, 0.f, 0.f));
	}
}

void FireBlock::OnCollisionStay(Object* other)
{
	ObjectType objType = other->GetObjectType();

	if (objType == ObjectType::Player)
	{
		auto playerStat = other->GetComponent<InfoComponent<PlayerInfo>>();
		playerStat->AddHp(-1);
	}
}

void FireBlock::Free()
{
}
