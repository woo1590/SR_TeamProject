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

PhysicsSystem* Scene::GetPhysicsStstem() const
{
	return PhysicsSys;
}

CameraManager* Scene::GetCameraManager() const
{
	return CameraMgr;
}

BlockManager* Scene::GetBlockManager() const
{
	return BlockMgr;
}

ChunkManager* Scene::GetChunkManager() const
{
	return ChunkMgr;
}

void Scene::Free()
{

}
