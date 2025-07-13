#include "pch.h"
#include "Rain.h"

//component
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

Rain::Rain(ObjectManager* owner, ObjectType objType)
	:Effect(owner,objType)
{
}

Rain::~Rain()
{
}

Rain* Rain::Create(ObjectManager* owner, ObjectType objType)
{
	Rain* Instance = new Rain(owner, objType);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT Rain::Ready_Object()
{
	Emitter rain;
	

	return S_OK;
}

void Rain::Update(_float dt)
{
	Object::Update(dt);
}

void Rain::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void Rain::Free()
{
	Object::Free();
}
