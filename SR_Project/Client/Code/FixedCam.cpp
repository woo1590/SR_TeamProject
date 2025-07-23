#include "pch.h"
#include "FixedCam.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"

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
	CameraActor::Ready_Object();

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

void FixedCam::SetPosition(_float x, _float y, _float z)
{
	GetComponent<TransformComponent>()->SetPosition(x, y, z);
}

void FixedCam::SetForward(_float x, _float y, _float z)
{
	auto transform = GetComponent<TransformComponent>();

	_vec3 forward{ x,y,z };
	D3DXVec3Normalize(&forward, &forward);

	transform->SetForward(forward);
}

void FixedCam::Free()
{
	Object::Free();
}
