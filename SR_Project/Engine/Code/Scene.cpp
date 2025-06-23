#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

HRESULT Scene::Ready_Scene()
{
	ObjectMgr = nullptr;
	Camera = nullptr;
	return S_OK;
}

void Scene::Free()
{

}
