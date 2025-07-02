#include "pch.h"
#include "CameraActor.h"
#include "EngineCore.h"
#include "RenderSystem.h"

//component	
#include "CameraComponent.h"
#include "ThirdcamComponent.h"
#include "TransformComponent.h"
#include "FreecamComponent.h"

CameraActor::CameraActor(ObjectManager* owner, ObjectType objType)
	:Object(owner,objType)
{
}

CameraActor::~CameraActor()
{
}

CameraActor* CameraActor::Create(ObjectManager* owner, ObjectType objType)
{
	CameraActor* Instance = new CameraActor(owner, objType);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT CameraActor::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto cam = AddComponent<CameraComponent>();
	auto controller = AddComponent<FreecamComponent>();

	EngineCore::GetInstance()->GetRenderSystem()->SetCamera(cam);

	return S_OK;
}

void CameraActor::Update(_float dt)
{
	Object::Update(dt);
}

void CameraActor::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void CameraActor::Free()
{
	Object::Free();
}
