#include "pch.h"
#include "TestScene.h"
#include "EngineCore.h"

//system
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "ImGuiManager.h"
#include "InputSystem.h"

//object
#include "BasicTerrain.h"
#include "TestObject.h"
#include "SkyBox.h"
#include "BaseCharacter.h"
#include "Player.h"
#include "CameraActor.h"
#include "UILoader.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"
#include "ThirdcamComponent.h"

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
	/*----------------Load ImGui----------------------*/
    EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"TestSceneUI", [this]() {this->ImGuiTestFunc();});

	/*------------------------------------------------*/
	//ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));

	//auto testObj = TestObject::Create(ObjectMgr, ObjectType::Player);
	//auto camActor = CameraActor::Create(ObjectMgr, ObjectType::Camera);
	//
	//ObjectMgr->AddObject(ObjectType::Player, testObj);
	//ObjectMgr->AddObject(ObjectType::Camera, camActor);
	//
	player = Player::Create(ObjectMgr, ObjectType::Player);
	ObjectMgr->AddObject(ObjectType::Player, player);
	
	UILoader loader;
	loader.LoadUI(ObjectMgr, player);
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

void TestScene::ImGuiTestFunc()
{
	/*ImGui::Begin("Simple Window");
	ImGui::Text("Hello, ImGui!");
	ImGui::End();*/
}

void TestScene::Free()
{
	Safe_Release(ObjectMgr);
	Scene::Free();
}