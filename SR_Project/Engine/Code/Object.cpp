#include "EnginePCH.h"
#include "Object.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "ObjectComponent.h"
#include "EngineCore.h"
#include "SceneManager.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"

#include "CollisionComponent.h"
#include "PhysicsComponent.h"

Object::Object(ObjectManager* owner, ObjectType objType)
	:owner(owner),ObjType(objType)
{
	
}

Object::~Object()
{
	
}

HRESULT Object::Ready_Object()
{
	Components.clear();
	ComponentMap.clear();

	return S_OK;
}

void Object::Update(float dt)
{
	for (const auto& comp : Components)
	{
		comp->Update(dt);
	}
}

void Object::Late_Update(float dt)
{
	for (const auto& comp : Components)
	{
		comp->Late_Update(dt);
	}
}

ObjectType Object::GetObjectType() const
{
	return ObjType;
}

void Object::SetDead()
{
	Is_Dead = true;
}

_bool Object::IsDead() const
{
	return Is_Dead;
}

ObjectManager* Object::GetOwner() const
{
	return owner;
}

Scene* Object::GetScene() const
{
	return owner->GetOwner();
}

void Object::UnRegister()
{
	auto collision = GetComponent<CollisionComponent>();
	if (collision)
		GetScene()->GetCollisionSystem()->UnRegisterCollision(collision);

	auto physics = GetComponent<PhysicsComponent>();
	if (physics)
		GetScene()->GetPhysicsStstem()->UnRegisterBody(physics);
}

void Object::Free()
{
	std::for_each(Components.begin(), Components.end(), [](ObjectComponent* comp)
		{
			Safe_Release(comp);
		});
	Components.clear();
	
	ComponentMap.clear();

}
