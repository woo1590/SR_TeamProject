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
#include "HPBarFront.h"
#include "Player.h"
#include "HPBarBack.h"
#include "CameraActor.h"
#include "LoadingUI.h"
#include "Cursor.h"
#include "InventoryPanel.h"
#include "UIDebugObj.h"
#include "HotBarBack.h"
#include "ExpBarFront.h"
#include "Slot.h"
#include "Emerald.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"
#include "ThirdcamComponent.h"
#include "HpComponent.h"
#include "PlayerInfoComponent.h"
#include "HpComponent.h"
#include "ExpComponent.h"

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
	ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));

	auto testObj = TestObject::Create(ObjectMgr, ObjectType::Player);
	auto camActor = CameraActor::Create(ObjectMgr, ObjectType::Camera);

	ObjectMgr->AddObject(ObjectType::Player, testObj);
	ObjectMgr->AddObject(ObjectType::Camera, camActor);

	player = Player::Create(ObjectMgr, ObjectType::Player);
	ObjectMgr->AddObject(ObjectType::Player, player);
	
	LoadUI();
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

void TestScene::LoadUI()
{
	const auto& info = player->GetComponent<PlayerInfoComponent>();
	
	auto hpBarFront = HPBarFront::Create(ObjectMgr);
	info->Attach(hpBarFront->GetComponent<HpComponent>());
	
	ObjectMgr->AddUIObject(hpBarFront);

	auto debugUI = UIDebugObj::Create(ObjectMgr);
	ObjectMgr->AddUIObject(debugUI);
	debugUI->SetPlayer(player);

	ObjectMgr->AddUIObject(HotBarBack::Create(ObjectMgr));
	
	auto expFront = ExpBarFront::Create(ObjectMgr);
	info->Attach(expFront->GetComponent<ExpComponent>());
	
	ObjectMgr->AddUIObject(expFront);
	ObjectMgr->AddUIObject(Slot::Create(ObjectMgr));

	ObjectMgr->AddUIObject(Emerald::Create(ObjectMgr));
}