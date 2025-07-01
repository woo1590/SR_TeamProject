#include "pch.h"
#include "FirstCam.h"

//component
#include "FreecamComponent.h"

FirstCam::FirstCam(ObjectManager* owner)
	:CameraActor(owner)
{
}

FirstCam::~FirstCam()
{
}

FirstCam* FirstCam::Create(ObjectManager* owner)
{
	FirstCam* Instance = new FirstCam(owner);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT FirstCam::Ready_Object()
{
	CameraActor::Ready_Object();
	auto fCam = AddComponent<FreecamComponent>();

	return S_OK;
}

void FirstCam::Update(_float dt)
{
	Object::Update(dt);
}

void FirstCam::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void FirstCam::Free()
{
	Object::Free();
}
