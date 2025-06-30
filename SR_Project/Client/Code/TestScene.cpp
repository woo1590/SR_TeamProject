#include "pch.h"
#include "TestScene.h"
#include "EngineCore.h"

//system
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "InputSystem.h"

//object
#include "BasicTerrain.h"
#include "TestObject.h"
#include "SkyBox.h"
#include "HPBarFront.h"
#include "Player.h"
#include "HPBarBack.h"
#include "LoadingUI.h"
#include "Cursor.h"
#include "InventoryPanel.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"
#include "HpComponent.h"

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

	//ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));
	//ObjectMgr->AddObject(ObjectType::Terrain, BasicTerrain::Create(ObjectMgr, ObjectType::Terrain));
	
	// ------------------------------------- UI Object -----------------------------------------------//
	player = Player::Create(ObjectMgr, ObjectType::Player);
	const auto& info = player->GetComponent<PlayerInfoComponent>();
	ObjectMgr->AddObject(ObjectType::Player, player);

	//ObjectMgr->AddObject(ObjectType::UI, LoadingUI::Create(ObjectMgr, ObjectType::UI));
	//ObjectMgr->AddObject(ObjectType::Player, TestObject::Create(ObjectMgr, ObjectType::Player));
	ObjectMgr->AddObject(ObjectType::UI, HPBarFront::Create(ObjectMgr, ObjectType::UI, info));
	ObjectMgr->AddObject(ObjectType::UI, HPBarBack::Create(ObjectMgr, ObjectType::UI));
	ObjectMgr->AddObject(ObjectType::UI, Cursor::Create(ObjectMgr, ObjectType::UI));
	//ObjectMgr->AddObject(ObjectType::UI, InventoryPanel::Create(ObjectMgr, ObjectType::UI));

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