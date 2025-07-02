#include "EnginePCH.h"
#include "Object.h"
#include "ObjectManager.h"
#include "ObjectComponent.h"
#include "EngineCore.h"
#include "SceneManager.h"

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

void Object::Free()
{
	std::for_each(Components.begin(), Components.end(), [](ObjectComponent* comp)
		{
			Safe_Release(comp);
		});
	Components.clear();
	
	std::for_each(ComponentMap.begin(), ComponentMap.end(), DeleteMap());
	ComponentMap.clear();

}
