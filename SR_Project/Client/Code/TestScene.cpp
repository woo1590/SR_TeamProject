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
#include "Monster.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"
#include "ThirdcamComponent.h"
#include "HpComponent.h"
#include "TestBlock.h"

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
	//ObjectMgr->AddObject(ObjectType::Terrain, BasicTerrain::Create(ObjectMgr, ObjectType::Terrain));
	//ObjectMgr->AddObject(ObjectType::Monster, BaseCharacter::Create(ObjectMgr, ObjectType::Monster));
	// ObjectMgr->AddObject(ObjectType::Terrain, BasicTerrain::Create(ObjectMgr, ObjectType::Terrain));
	//LoadBlock();
	ObjectMgr->AddObject(ObjectType::Player, TestObject::Create(ObjectMgr, ObjectType::Player));

	auto testObj = TestObject::Create(ObjectMgr, ObjectType::Player);
	auto camActor = CameraActor::Create(ObjectMgr, ObjectType::Camera);
	//camActor->GetComponent<ThirdcamComponent>()->SetFollowTarget(testObj);

	ObjectMgr->AddObject(ObjectType::Player, testObj);
	ObjectMgr->AddObject(ObjectType::Camera, camActor);
	
	/*------------------Load UI------------------------*/
	player = Player::Create(ObjectMgr, ObjectType::Player);	
	const auto& info = player->GetComponent<PlayerInfoComponent>();
	ObjectMgr->AddObject(ObjectType::Player, player);

	ObjectMgr->AddObject(ObjectType::Monster, Monster::Create(ObjectMgr, ObjectType::Monster));

	//ObjectMgr->AddObject(ObjectType::UI, HPBarFront::Create(ObjectMgr, ObjectType::UI, info));
	//ObjectMgr->AddObject(ObjectType::UI, HPBarBack::Create(ObjectMgr, ObjectType::UI));

	//ObjectMgr->AddObject(ObjectType::Monster, BaseCharacter::Create(ObjectMgr, ObjectType::Monster));
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

void TestScene::LoadBlock()
{
	HANDLE hFile(nullptr);
	hFile = CreateFile(L"../../Reference/MapData/TestScene.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(EngineCore::GetInstance()->GetWindowHandle(), L"Load Fail", _T("Fail"), MB_OK);
		return;
	}

	ObjectMgr->ClearList(ObjectType::Block);
	Blocks.clear();
	Blocks.shrink_to_fit();

	DWORD dwByte(0);
	BlockData newBlock;
	while (TRUE)
	{
		if (!ReadFile(hFile, &newBlock, sizeof(BlockData), &dwByte, nullptr)) return;
		if (dwByte == 0) break;

		auto block = TestBlock::Create(ObjectMgr, ObjectType::Block, newBlock.Type);
		block->GetComponent<TransformComponent>()->SetPosition(newBlock.Pos);
		ObjectMgr->AddObject(ObjectType::Block, block);
		Blocks.push_back(newBlock);
	}

	CloseHandle(hFile);
	MessageBox(EngineCore::GetInstance()->GetWindowHandle(), L"Load Success", _T("Success"), MB_OK);
}