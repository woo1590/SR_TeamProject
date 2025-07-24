#include "pch.h"
#include "DirectionLight.h"
#include "ObjectManager.h"
#include "Player.h"

//component
#include "TransformComponent.h"
#include "LightComponent.h"

DirectionLight::DirectionLight(ObjectManager* owner, ObjectType objType)
	:Object(owner,objType)
{
}

DirectionLight::~DirectionLight()
{
}

DirectionLight* DirectionLight::Create(ObjectManager* owner, ObjectType objType)
{
	DirectionLight* Instance = new DirectionLight(owner, objType);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT DirectionLight::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();


	return S_OK;
}

void DirectionLight::Free()
{
	Object::Free();
}
