#include "EnginePCH.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

HRESULT Scene::Ready_Scene()
{
	return S_OK;
}

ObjectManager* Scene::GetObjectManager() const
{
	return ObjectMgr;
}

EventSystem* Scene::GetEventSystem() const
{
	return EventSys;
}

CollisionSystem* Scene::GetCollisionSystem() const
{
	return CollisionSys;
}

CameraManager* Scene::GetCameraManager() const
{
	return CameraMgr;
}

void Scene::Free()
{

}
