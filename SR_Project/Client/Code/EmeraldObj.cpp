#include "pch.h"
#include "EmeraldObj.h"
#include "ItemComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"
#include "Scene.h"
#include "InfoComponent.h"
#include "ObjectManager.h"

EmeraldObj::EmeraldObj(ObjectManager* owner, ObjectType objType)
	:Object(owner, objType)
{
}

EmeraldObj::~EmeraldObj()
{
}

EmeraldObj* EmeraldObj::Create(ObjectManager* owner, ObjectType objType)
{
	EmeraldObj* Instance = new EmeraldObj(owner, objType);

	if (FAILED(Instance->Ready_Object(owner, objType)))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT EmeraldObj::Ready_Object(ObjectManager* owner, ObjectType objType)
{
	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto item = AddComponent<ItemComponent>();
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.7f, 1.f, 0.1f);

	auto collision = AddComponent<CollisionComponent>();
	collision->AddCollider<AABBCollider>();
	GetScene()->GetCollisionSystem()->RegisterCollision(collision);
	collision->SetLayer(LAYER_DEFAULT);
	collision->SetMask(LAYER_DEFAULT | LAYER_PLAYER);
	collision->SetSize(_vec3(2.f, 2.f, 2.f));

	auto physics = AddComponent<PhysicsComponent>();
	GetScene()->GetPhysicsStstem()->RegisterBody(physics);
	physics->SetMass(1.f);

	auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMesh("Cube_Mesh");
	renderer->SetMaterial("EmeraldCube_Mtrl");

	return S_OK;
}

void EmeraldObj::Update(_float dt)
{
	Object::Update(dt);
	auto transform = GetComponent<TransformComponent>();
	_vec3 rot = transform->GetRotate();
	transform->SetRotate(rot.x, rot.y + 2 * dt, rot.z);

	auto physics = GetComponent<PhysicsComponent>();
	if(physics->IsGrounded())
		physics->SetVelocity(0,0,0);
}

void EmeraldObj::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void EmeraldObj::Throw()
{
	auto physics = GetComponent<PhysicsComponent>();
	physics->SetVelocity(rand() % 10 - 5, 15.f, rand() % 10 - 5);

	owner->AddObject(ObjectType::Item, this);
}

void EmeraldObj::Free()
{
	Object::Free();
}
