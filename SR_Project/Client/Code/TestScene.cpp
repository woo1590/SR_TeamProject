#include "pch.h"
#include "TestScene.h"
#include "EngineCore.h"

//system
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "ResourceManager.h"

//object
#include "BasicTerrain.h"
#include "TestObject.h"
#include "SkyBox.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}

TestScene* TestScene::Create()
{
	TestScene* Instance = new TestScene;

	return Instance;
}

void TestScene::Load()
{
	ObjectMgr = ObjectManager::Create(this);

	ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));
	ObjectMgr->AddObject(ObjectType::Terrain, BasicTerrain::Create(ObjectMgr, ObjectType::Terrain));
	ObjectMgr->AddObject(ObjectType::Player, TestObject::Create(ObjectMgr, ObjectType::Player));
	/*----------------------------------------------------------------------------------------------*/

	
}

void TestScene::Update(float dt)
{
	ObjectMgr->Update(dt);
}

void TestScene::Late_Update(float dt)
{
	ObjectMgr->Late_Update(dt);
}

void TestScene::Unload()
{

}

void TestScene::Free()
{
	Safe_Release(ObjectMgr);
	Scene::Free();
}