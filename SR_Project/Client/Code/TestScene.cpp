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
#include "PhysicsSystem.h"

//object
#include "TestObject.h"
#include "SkyBox.h"
#include "BaseCharacter.h"
#include "Player.h"
#include "CameraActor.h"
#include "LoadingUI.h"
#include "Cursor.h"
#include "InventoryPanel.h"
#include "Zombie.h"
#include "FirstCam.h"
#include "ThirdCam.h"
#include "UIDebugObj.h"
#include "HotBarBack.h"
#include "ExpBarFront.h"
#include "QuickSlot.h"
#include "Emerald.h"
#include "UILoader.h"
#include "DirectionLight.h"
#include "Skeleton.h"
#include "Creeper.h"
//component
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"
#include "ThirdcamComponent.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"

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
	ObjectMgr		= ObjectManager::Create(this);
	EventSys		= EventSystem::Create(this);
	CollisionSys	= CollisionSystem::Create(this);
	CameraMgr		= CameraManager::Create(this);
	PhysicsSys		= PhysicsSystem::Create(this);

#ifdef USE_IMGUI
	/*----------------Load ImGui----------------------*/
	EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"TestSceneUI", [this]() {this->TestSceneImGui();});

#endif
	/*----------------Load Camera---------------------*/
	LoadBlock();
	player = Player::Create(ObjectMgr, ObjectType::Player);
	player->GetComponent<TransformComponent>()->SetPosition(-5.f, 100.f, -5.f);
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
	
	//ObjectMgr->AddObject(ObjectType::Camera, fCam);
	//ObjectMgr->AddObject(ObjectType::Camera, tCam);

	/*------------------------------------------------*/
	ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));
	ObjectMgr->AddObject(ObjectType::Monster, Zombie::Create(ObjectMgr, ObjectType::Monster));
	ObjectMgr->AddObject(ObjectType::Monster, Skeleton::Create(ObjectMgr, ObjectType::Monster));
	ObjectMgr->AddObject(ObjectType::Monster, Creeper::Create(ObjectMgr, ObjectType::Monster));
	

	/*------------------Load UI------------------------*/
	//LoadUI();
	/*-------------------------------------------------*/
	
	UILoader loader;
	loader.LoadUI(ObjectMgr, player);
}

void TestScene::Update(float dt)
{
	ObjectMgr->Update(dt);
	//PhysicsSys->Update(dt);

	/*-------------ī�޶� ��ȯ �׽�Ʈ �ڵ�-------------*/
	auto Input = EngineCore::GetInstance()->GetInputSystem();
	if (Input->IsKeyPressed(NUM1))
		CameraMgr->SetMainCamera(L"First_Camera");
	if (Input->IsKeyPressed(NUM2))
		CameraMgr->SetMainCamera(L"Third_Camera");

	/*-------------���� ��ŷ �׽�Ʈ �ڵ�--------------*/

	if (Input->IsKeyPressed(Q))
	{
		for (auto& Dst : ObjectMgr->GetObjectList(ObjectType::DynamicBlock))
		{
			if (static_cast<DynamicBlock*>(Dst)->GetType() == LeverSwitch)
				static_cast<DynamicBlock*>(Dst)->SetActivate();
		}
	}
}

void TestScene::Late_Update(float dt)
{
	ObjectMgr->Late_Update(dt);
	CollisionSys->Late_Update();
}

void TestScene::Unload()
{

}

#ifdef USE_IMGUI
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
#endif

void TestScene::Free()
{
	Safe_Release(ObjectMgr);
	Safe_Release(EventSys);
	Safe_Release(CollisionSys);
	Safe_Release(CameraMgr);
	Safe_Release(PhysicsSys);

	Scene::Free();
}

void TestScene::LoadBlock()
{
	HANDLE hFile(nullptr);
	hFile = CreateFile(L"../../Reference/MapData/MapV1.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(EngineCore::GetInstance()->GetWindowHandle(), L"Load Fail", _T("Fail"), MB_OK);
		return;
	}

	ObjectMgr->ClearList(ObjectType::StaticBlock); staticBlocks.clear(); staticBlocks.shrink_to_fit();
	ObjectMgr->ClearList(ObjectType::DynamicBlock); dynamicBlocks.clear(); dynamicBlocks.shrink_to_fit();

	DWORD dwByte(0), dwSBTot(0), dwDBTot(0);
	DWORD SBSize(0), DBSize(0);
	SB newSBlock; DB newDBlock;

	if (!ReadFile(hFile, &SBSize, sizeof(DWORD), &dwByte, nullptr)) return;
	if (!ReadFile(hFile, &DBSize, sizeof(DWORD), &dwByte, nullptr)) return;

	for (DWORD i = 0; i < SBSize; ++i)
	{
		if (!ReadFile(hFile, &newSBlock, sizeof(SB), &dwByte, nullptr)) return;

		auto sBlock = StaticBlock::Create(ObjectMgr, ObjectType::StaticBlock, newSBlock.Type, newSBlock.Dir);
		sBlock->GetComponent<TransformComponent>()->SetPosition(newSBlock.Pos);

		ObjectMgr->AddObject(ObjectType::StaticBlock, sBlock);
		staticBlocks.push_back(newSBlock);
	}

	for (DWORD i = 0; i < DBSize; ++i)
	{
		if (!ReadFile(hFile, &newDBlock, sizeof(DB), &dwByte, nullptr)) return;

		Object* dBlock = nullptr;

		if (newDBlock.Type == DynamicBlockType::IronCages)
		{
			int count = 0;
			ReadFile(hFile, &count, sizeof(int), &dwByte, nullptr);

			dBlock = DynamicBlock::Create(ObjectMgr, ObjectType::DynamicBlock, newDBlock.Type, newDBlock.Dir, count);
			dBlock->GetComponent<TransformComponent>()->SetPosition(newDBlock.Pos);

			int vecSize = 0;
			ReadFile(hFile, &vecSize, sizeof(int), &dwByte, nullptr);
			if (vecSize > 0)
			{
				std::vector<int> ids(vecSize);
				ReadFile(hFile, ids.data(), sizeof(int) * vecSize, &dwByte, nullptr);
				for (int id : ids)
					static_cast<DynamicBlock*>(dBlock)->AddID(id);
			}
		}
		else
		{
			dBlock = DynamicBlock::Create(ObjectMgr, ObjectType::DynamicBlock, newDBlock.Type, newDBlock.Dir, Count);
			dBlock->GetComponent<TransformComponent>()->SetPosition(newDBlock.Pos);

			if (newDBlock.Type == DynamicBlockType::LeverSwitch)
			{
				int id = 0;
				ReadFile(hFile, &id, sizeof(int), &dwByte, nullptr);
				static_cast<DynamicBlock*>(dBlock)->SetID(id);
			}
		}

		ObjectMgr->AddObject(ObjectType::DynamicBlock, dBlock);
		dynamicBlocks.push_back(newDBlock);
	}

	CloseHandle(hFile);
	MessageBox(EngineCore::GetInstance()->GetWindowHandle(), L"Load Success", _T("Success"), MB_OK);
}
