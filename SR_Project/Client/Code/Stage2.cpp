#include "pch.h"
#include "Stage2.h"
#include "EngineCore.h"
#include "LoadingScene.h"
#include "SceneManager.h"
#include "ChangeScene.h"
#include "GameManager.h"

//system
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "ImGuiManager.h"
#include "InputSystem.h"
#include "CollisionSystem.h"
#include "CameraManager.h"
#include "PhysicsSystem.h"
#include "StaticGrid.h"
#include "BlockManager.h"
#include "ChunkManager.h"
#include "UIManager.h"
#include "ChunkManager.h"
#include "SoundManager.h"
#include "ChunkLoader.h"

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
#include "StaticBlock.h"
#include "DynamicBlock.h"
#include "RedGolem.h"
#include "Chunk.h"
#include "SpawnTriggerBox.h"
#include "Tnt.h"
#include "Rain.h"
#include "Slime.h"
#include "JungleZombie.h"
#include "JungleSkeleton.h"
#include "BloodEffect.h"
#include "PurpleSlime.h"
#include "SpriteEffect.h"
#include "Npc.h"
#include "Ender.h"
#include "Spear.h"
#include "DeadEffect.h"
#include "InventoryCam.h"
#include "WayPointCam.h"

//component
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"
#include "SpriteRenderer.h"
#include "ThirdcamComponent.h"


Stage2::Stage2()
	:Scene()
{
}

Stage2::~Stage2()
{
}

Stage2* Stage2::Create()
{
	Stage2* Instance = new Stage2;

	return Instance;
}

void Stage2::Load()
{
	auto game = GameManager::GetInstance();
#ifdef USE_IMGUI
	EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"Debug", [this]() {this->DebugIMGUI();});
	EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"way", [this]() {this->WayPointEdit();});
#endif

	/*-------------------------Create System-----------------------------*/
	{
		EngineCore::GetInstance()->GetSoundManager()->PlayBGM("Stage2BGM");

		Grid = StaticGrid::Create(this);
		ObjectMgr = ObjectManager::Create(this);
		CollisionSys = CollisionSystem::Create(this);
		CameraMgr = CameraManager::Create(this);
		PhysicsSys = PhysicsSystem::Create(this);
		BlockMgr = BlockManager::Create(this);
		ChunkMgr = ChunkManager::Create(this);
		uiMgr = UIManager::Create(this);
	}

	/*-------------------------Create Camera-----------------------------*/
	{
		if (game->GetPlayer())
		{
			player = game->GetPlayer();
			player->SetOwner(ObjectMgr);
			player->AddRef();
		}
		else
		{
			player = Player::Create(ObjectMgr, ObjectType::Player);
			game->SetPlayer(player);
		}

		ObjectMgr->AddObject(ObjectType::Player, player);

		auto fCam = FirstCam::Create(ObjectMgr);
		auto tCam = ThirdCam::Create(ObjectMgr);
		auto wCam = WayPointCam::Create(ObjectMgr);
		auto inventoryCam = InventoryCam::Create(ObjectMgr);

		CameraMgr->AddCamera(L"First_Camera", fCam);
		CameraMgr->AddCamera(L"Third_Camera", tCam);
		CameraMgr->AddCamera(L"Way_Camera", wCam);
		CameraMgr->AddCamera(L"Inventory_Camera", inventoryCam);
		tCam->SetTarget(player);

		CameraMgr->SetMainCamera(L"Third_Camera");

		ObjectMgr->AddObject(ObjectType::Camera, fCam);
		ObjectMgr->AddObject(ObjectType::Camera, tCam);
		ObjectMgr->AddObject(ObjectType::Camera, wCam);
		ObjectMgr->AddObject(ObjectType::UICamera, inventoryCam);

		wayCam = wCam;
		wCam->AddRef();
	}

	/*----------------Load Chunk----------------*/
	{
		auto chunkload = EngineCore::GetInstance()->GetChunkLoader();
		ChunkMgr->SetChunk(chunkload->GetChunks());
		BlockMgr->LoadDB("Stage2");

		for (auto& dynamic : ObjectMgr->GetObjectList(ObjectType::DynamicBlock))
		{
			auto lever = static_cast<DynamicBlock*>(dynamic);
			if (lever->GetType() != DynamicBlockType::LeverSwitch && lever->GetType() != DynamicBlockType::BasicChest) continue;
			lever->SetTarget(player);
		}

		Grid->InsertBlock();
	}

	/*------------------Create Object---------------*/
	{
		UILoader loader;
		loader.LoadUI(ObjectMgr);

		auto skybox = SkyBox::Create(ObjectMgr, ObjectType::SkyBox);
		skybox->GetComponent<MeshRenderer>()->SetMaterial("Stage2SkyBox_Mtrl");
	
		ObjectMgr->AddObject(ObjectType::SkyBox, skybox);
		player->GetComponent<TransformComponent>()->SetPosition(110.f, 120.f, 170.f);
	}

	ChangeState(Stage2Stage::Stage2Intro);
}

void Stage2::Update(_float dt)
{
	ObjectMgr->Update(dt);
	PhysicsSys->Update(dt);
	ChunkMgr->IsChunkBoundary(CameraMgr->GetMainCamera()->GetOwner()->GetComponent<TransformComponent>()->GetPosition());
	uiMgr->Update(dt);

	switch (currState)
	{
	case Stage2::Stage2Stage::Stage2Intro:
	{
		if (stage2IntroTimer >= stage2IntroDuration)
			ChangeState(Stage2Stage::Play);

		stage2IntroTimer += dt;

	}break;
	case Stage2::Stage2Stage::BossIntro:
	{

	}break;
	case Stage2::Stage2Stage::ActiveBridge:
	{

	}break;
	case Stage2::Stage2Stage::Play:
	{

	}break;
	default:
		break;
	}

	{
		auto Input = EngineCore::GetInstance()->GetInputSystem();

		if (Input->IsKeyPressed(F))
			CameraMgr->SetMainCamera(L"First_Camera");

		if (Input->IsKeyPressed(T))
			CameraMgr->SetMainCamera(L"Third_Camera");

		if (Input->IsKeyPressed(NUM4))
		{
			auto command = ChangeScene::Create(LOADID::Village);
			EngineCore::GetInstance()->RegisterCommand(command);
			GameManager::GetInstance()->ClearScene(LOADID::Stage2);
		}
	}
}

void Stage2::Late_Update(_float dt)
{
	ObjectMgr->Late_Update(dt);
}

void Stage2::Unload()
{
}

#ifdef USE_IMGUI 
void Stage2::DebugIMGUI()
{
	ImGui::Begin("Player Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (!player)
		return;

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

		ImGui::EndTable();
	}
	ImGui::End();
}
void Stage2::WayPointEdit()
{
	bool p_open = true;

	auto cam = dynamic_cast<FirstCam*>(CameraMgr->GetMainCamera()->GetOwner());

	if (!ImGui::Begin("Waypoint Editor", &p_open))
	{
		ImGui::End();
		return;
	}

	if (!cam)
	{
		ImGui::Text("Camera not selected.");
		ImGui::End();
		return;
	}

	ImGui::DragFloat("Total Duration", wayCam->GetDuration(), 0.1f, 1.f, 300.f);
	ImGui::Separator();


	if (ImGui::Button("Add Waypoint"))
	{
		_vec3 pos = cam->GetComponent<TransformComponent>()->GetPosition();
		_vec3 dir = cam->GetComponent<TransformComponent>()->GetFoward();

		wayCam->AddWaypoint({ pos, dir });
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear All"))
	{
		wayCam->Clear();
	}
	ImGui::Separator();
	if (ImGui::Button("Start"))
	{
		wayCam->Start();
		CameraMgr->SetMainCamera(L"Way_Camera");
	}
	ImGui::Separator();
	if (ImGui::Button("Stop"))
	{
		wayCam->Stop();
		CameraMgr->SetMainCamera(L"First_Camera");
	}
	ImGui::Separator();

	auto& waypoints = wayCam->GetWaypoints();
	for (int i = 0; i < waypoints.size(); ++i)
	{
		ImGui::PushID(i);

		ImGui::Text("Waypoint %d", i);

		ImGui::DragFloat3("Position", (float*)&waypoints[i].position, 0.1f);
		ImGui::DragFloat3("Direction", (float*)&waypoints[i].lookDir, 0.01f);


		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			wayCam->RemoveWaypoint(i);
			ImGui::PopID();
			break;
		}

		ImGui::Separator();
		ImGui::PopID();

	}

	if (ImGui::Button("Save to File"))
	{
		wayCam->SaveWaypoints("../Resource/Data/Stage2Waypoint.dat");
	}
	ImGui::SameLine();
	if (ImGui::Button("Load from File"))
	{
		wayCam->LoadWaypoints("../Resource/Data/Stage2Waypoint.dat");
	}

	ImGui::End();
}
#endif

void Stage2::ChangeState(Stage2Stage state)
{
	switch (state)
	{
	case Stage2::Stage2Stage::Stage2Intro:
	{
		currState = Stage2Stage::Stage2Intro;
		stage2IntroDuration = 20.f;
		stage2IntroTimer = 0.f;

		CameraMgr->SetMainCamera(L"Way_Camera");
		auto cam = static_cast<WayPointCam*>(CameraMgr->GetMainCamera()->GetOwner());

		cam->SetDuration(stage2IntroDuration);
		cam->LoadWaypoints("../Resource/Data/Stage2Waypoint.dat");
		cam->Start();

	}break;
	case Stage2::Stage2Stage::BossIntro:
	{


	}break;
	case Stage2::Stage2Stage::Play:
	{
		currState = Stage2Stage::Play;

		CameraMgr->SetMainCamera(L"Third_Camera");

	}break;
	case Stage2::Stage2Stage::ActiveBridge:
	{


	}break;
	default:
		break;
	}
}

void Stage2::Free()
{
	Safe_Release(ObjectMgr);
	Safe_Release(CollisionSys);
	Safe_Release(PhysicsSys);
	Safe_Release(CameraMgr);
	Safe_Release(BlockMgr);
	Safe_Release(Grid);
	Safe_Release(uiMgr);
	Safe_Release(ChunkMgr);

	Scene::Free();
}
