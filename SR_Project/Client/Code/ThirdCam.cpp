#include "pch.h"
#include "ThirdCam.h"

//component
#include "ThirdcamComponent.h"

ThirdCam::ThirdCam(ObjectManager* owner)
	:CameraActor(owner)
{
}

ThirdCam::~ThirdCam()
{
}

ThirdCam* ThirdCam::Create(ObjectManager* owner)
{
	ThirdCam* Instance = new ThirdCam(owner);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT ThirdCam::Ready_Object()
{
	CameraActor::Ready_Object();
	auto tCam = AddComponent<ThirdcamComponent>();

	return S_OK;
}

void ThirdCam::Update(_float dt)
{
	Object::Update(dt);
}

void ThirdCam::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void ThirdCam::SetTarget(Object* target)
{
	GetComponent<ThirdcamComponent>()->SetFollowTarget(target);
}

void ThirdCam::SetOffset(_vec3 offset)
{
	GetComponent<ThirdcamComponent>()->SetOffset(offset);
}

void ThirdCam::Free()
{
	Object::Free();
}
