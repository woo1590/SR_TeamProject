#include "pch.h"
#include "TestScene.h"
#include "EngineCore.h"

//system
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "ImGuiManager.h"
#include "InputSystem.h"
#include "EventSystem.h"
#include "CollisionSystem.h"
#include "CameraManager.h"

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
#include "FirstCam.h"
#include "ThirdCam.h"
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
#include "TestBlock.h"
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
	EventSys = EventSystem::Create(this);
	CollisionSys = CollisionSystem::Create(this);
	CameraMgr = CameraManager::Create(this);

	/*----------------Load ImGui----------------------*/
	EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"TestSceneUI", [this]() {this->TestSceneImGui();});

	/*----------------Load Camera---------------------*/
	player = Player::Create(ObjectMgr, ObjectType::Player);
	player->GetComponent<TransformComponent>()->SetPosition(0.f, 20.f, 0.f);
	ObjectMgr->AddObject(ObjectType::Player, player);

	auto fCam = FirstCam::Create(ObjectMgr);
	auto tCam = ThirdCam::Create(ObjectMgr);

	CameraMgr->AddCamera(L"First_Camera", fCam);
	CameraMgr->AddCamera(L"Third_Camera", tCam);
	tCam->SetTarget(player);

	CameraMgr->SetMainCamera(L"First_Camera");
	EventSys->SetCamera();

	ObjectMgr->AddObject(ObjectType::Camera, fCam);
	ObjectMgr->AddObject(ObjectType::Camera, tCam);

	/*------------------------------------------------*/
	ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));
	//LoadBlock();

	ObjectMgr->AddObject(ObjectType::Monster, Monster::Create(ObjectMgr, ObjectType::Monster));

	/*------------------Load UI------------------------*/
	//LoadUI();
	/*----------------------------------------------------------------------------------------------*/
}

void TestScene::Update(float dt)
{
	ObjectMgr->Update(dt);
	EventSys->Update();

	/*-------------ī�޶� ��ȯ �׽�Ʈ �ڵ�-------------*/
	auto Input = EngineCore::GetInstance()->GetInputSystem();
	if (Input->IsKeyPressed(NUM1))
		CameraMgr->SetMainCamera(L"First_Camera");
	if (Input->IsKeyPressed(NUM2))
		CameraMgr->SetMainCamera(L"Third_Camera");

	/*-------------���� ��ŷ �׽�Ʈ �ڵ�--------------*/
}

void TestScene::Late_Update(float dt)
{
	ObjectMgr->Late_Update(dt);
	CollisionSys->Late_Update();
}

void TestScene::Unload()
{

}

void TestScene::TestSceneImGui()
{
	ImGui::Begin("Player Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	_vec3 pos = player->GetComponent<TransformComponent>()->GetPosition();
	_vec3 camPos = CameraMgr->GetMainCamera()->GetOwner()->GetComponent<TransformComponent>()->GetPosition();

	if (ImGui::BeginTable("##PosTable", 2,
		ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg))
	{
		ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 120.0f);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::TextUnformatted("Player Position");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%.2f, %.2f, %.2f", pos.x, pos.y, pos.z);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::TextUnformatted("Camera Position");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%.2f, %.2f, %.2f", camPos.x, camPos.y, camPos.z);

		// ī�޶� ��ȯ ��ư
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::TextUnformatted("Switch Camera");
		ImGui::TableSetColumnIndex(1);
		if (ImGui::Button("First")) {
			CameraMgr->SetMainCamera(L"First_Camera");
		}
		ImGui::SameLine();
		if (ImGui::Button("Third")) {
			CameraMgr->SetMainCamera(L"Third_Camera");
		}

		ImGui::EndTable();
	}

	ImGui::End();

}

void TestScene::Free()
{
	Safe_Release(ObjectMgr);
	Safe_Release(EventSys);
	Safe_Release(CollisionSys);
	Safe_Release(CameraMgr);

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

		auto block = TestBlock::Create(ObjectMgr, ObjectType::Block, newBlock.Type, newBlock.Dir);
		block->GetComponent<TransformComponent>()->SetPosition(newBlock.Pos);
		ObjectMgr->AddObject(ObjectType::Block, block);

		Blocks.push_back(newBlock);
	}

	CloseHandle(hFile);
	MessageBox(EngineCore::GetInstance()->GetWindowHandle(), L"Load Success", _T("Success"), MB_OK);
}

void TestScene::LoadUI()
{
	const auto& info = player->GetComponent<PlayerInfoComponent>();

	auto hpBarFront = HPBarFront::Create(ObjectMgr);
//	info->Attach(hpBarFront->GetComponent<HpComponent>());

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