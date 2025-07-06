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

CollisionSystem* Scene::GetCollisionSystem() const
{
	return CollisionSys;
}

PhysicsSystem* Scene::GetPhysicsStstem() const
{
	return PhysicsSys;
}

CameraManager* Scene::GetCameraManager() const
{
	return CameraMgr;
}

StaticGrid* Scene::GetStaticGrid() const
{
	return Grid;
}

void Scene::Free()
{

}
