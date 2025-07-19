#include "pch.h"
#include "LaserEffect.h"
#include "SpriteRenderer.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "Material.h"
#include "CollisionComponent.h"
#include "Scene.h"
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "InfoComponent.h"

LaserEffect::LaserEffect(ObjectManager* owner, ObjectType objType)
	:Object(owner,objType)
{
}

LaserEffect::~LaserEffect()
{
}

LaserEffect* LaserEffect::Create(ObjectManager* owner, ObjectType objType)
{
	LaserEffect* Instance = new LaserEffect(owner, objType);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT LaserEffect::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(_vec3(1.f, 1.f, 1.f));

	auto collision = AddComponent<CollisionComponent>();
	collision->AddCollider<AABBCollider>();
	GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
	collision->SetLayer(LAYER_PROJECTILE);
	collision->SetMask(LAYER_PLAYER);
	collision->SetCollisionEnter([this](Object* other) {this->OnCollisionStay(other); });
	collision->SetSize(_vec3(1.f, 1.f, 1.f));

	auto physics = AddComponent<PhysicsComponent>();
	GetScene()->GetPhysicsStstem()->RegisterBody(physics);
	physics->SetKinematic(true);
	physics->SetMass(1.f);

	auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMesh("Quad_Mesh");
	renderer->SetMaterial("Laser_Mtrl2");
	auto mtrl = renderer->GetMaterial();
	mtrl->SetFloat("time", ElapsedTime);
	mtrl->SetVec3("color", _vec3(0.7f, 0.f, 0.7f));
	return S_OK;
}

void LaserEffect::Update(_float dt)
{
	Object::Update(dt);

	auto renderer = GetComponent<MeshRenderer>();
	auto mtrl = renderer->GetMaterial();
	ElapsedTime += dt;
	mtrl->SetFloat("time", ElapsedTime);
}

void LaserEffect::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void LaserEffect::Free()
{
	Object::Free();
}

void LaserEffect::SetActive(_bool active)
{
	Active = active;

	auto renderer = GetComponent<MeshRenderer>();
	if (Active)
	{
		renderer->SetRenderID(RENDER_ID::Render_Alpha);
	}
	else
	{
		renderer->SetRenderID(RENDER_ID::Render_None);
	}
}

void LaserEffect::OnCollisionStay(Object* other)
{
	ObjectType objType = other->GetObjectType();

	if (objType == ObjectType::Player)
	{
		auto playerStat = other->GetComponent<InfoComponent<PlayerInfo>>();
		playerStat->AddHp(-1);
	}
}
