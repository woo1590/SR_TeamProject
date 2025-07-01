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

HRESULT CameraActor::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto cam = AddComponent<CameraComponent>();

	return S_OK;
}

void CameraActor::Free()
{

}
