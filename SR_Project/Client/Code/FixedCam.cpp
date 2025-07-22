#include "pch.h"
#include "FixedCam.h"

FixedCam::FixedCam(ObjectManager* owner)
	:CameraActor(owner)
{
}

FixedCam::~FixedCam()
{
}

FixedCam* FixedCam::Create(ObjectManager* owner)
{
	FixedCam* Instance = new FixedCam(owner);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT FixedCam::Ready_Object()
{
	return S_OK;
}

void FixedCam::Update(_float dt)
{
	Object::Update(dt);
}

void FixedCam::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void FixedCam::Free()
{
	Object::Free();
}
