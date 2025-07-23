#include "pch.h"
#include "Stage1.h"
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
#include "WayPointCam.h"
#include "InventoryCam.h"
#include "Pig.h"

//component
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"
#include "SpriteRenderer.h"
#include "ThirdcamComponent.h"

Stage1::Stage1()
	:Scene()
{
}

Stage1::~Stage1()
{
}

Stage1* Stage1::Create()
{
	Stage1* Instance = new Stage1;

	return Instance;
}

void Stage1::Load()
{
	auto game = GameManager::GetInstance();
#ifdef USE_IMGUI
	EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"Debug", [this]() {this->DebugIMGUI();});
	EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"Waypoint", [this]() {this->WayPointEdit();});
#endif

	/*-------------------------Create System-----------------------------*/
	{
		EngineCore::GetInstance()->GetSoundManager()->PlayBGM("Stage1BGM");

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
		BlockMgr->LoadDB("Stage1");

		Grid->InsertBlock();
	}

	/*------------------Create Object---------------*/
	{
		UILoader loader;
		loader.LoadUI(ObjectMgr);

		auto skybox = SkyBox::Create(ObjectMgr, ObjectType::SkyBox);
		skybox->GetComponent<MeshRenderer>()->SetMaterial("Stage1SkyBox_Mtrl");
		ObjectMgr->AddObject(ObjectType::SkyBox, skybox);

		player->GetComponent<TransformComponent>()->SetPosition(110.f, 120.f, 170.f);

		SetTriggerBox();
		
		ObjectMgr->AddObject(ObjectType::Neutral, Pig::Create(ObjectMgr, ObjectType::Neutral));
	}
	ChangeState(Stage1State::Stage1Intro);
}

void Stage1::Update(_float dt)
{
	ObjectMgr->Update(dt);
	PhysicsSys->Update(dt);
	ChunkMgr->IsChunkBoundary(CameraMgr->GetMainCamera()->GetOwner()->GetComponent<TransformComponent>()->GetPosition());
	uiMgr->Update(dt);

	switch (currState)
	{
	case Stage1::Stage1State::Stage1Intro:
	{
		if (stage1IntroTimer >= stage1IntroDuration)
			ChangeState(Stage1State::Play);

		stage1IntroTimer += dt;
	}break;
	case Stage1::Stage1State::BossIntro:
	{

	}break;
	case Stage1::Stage1State::Play:
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

		if (Input->IsKeyPressed(NUM9))
			EngineCore::GetInstance()->SetDebugMode(true);

		if (Input->IsKeyPressed(TAB))
			ChangeState(Stage1State::Play);


		if (Input->IsKeyPressed(NUM4))
		{
			auto command = ChangeScene::Create(LOADID::Village);
			EngineCore::GetInstance()->RegisterCommand(command);
			GameManager::GetInstance()->ClearScene(LOADID::Stage1);
		}
	}
}

void Stage1::Late_Update(_float dt)
{
	ObjectMgr->Late_Update(dt);
}

void Stage1::Unload()
{
	EngineCore::GetInstance()->GetSoundManager()->Stop("Stage1BGM");
}

#ifdef USE_IMGUI 
void Stage1::DebugIMGUI()
{
	ImGui::Begin("Player Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (!player)
		return;

	_vec3 pos = player->GetComponent<TransformComponent>()->GetPosition();
	_vec3 camPos = CameraMgr->GetMainCamera()->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
	_vec3 forward = CameraMgr->GetMainCamera()->GetOwner()->GetComponent<TransformComponent>()->GetFoward();

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

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::TextUnformatted("Camera Dir");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%.2f, %.2f, %.2f", forward.x, forward.y, forward.z);

		ImGui::EndTable();
	}
	ImGui::End();
}

void Stage1::WayPointEdit()
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
		wayCam->SaveWaypoints("../Resource/Data/Stage1Waypoint.dat");
	}
	ImGui::SameLine();
	if (ImGui::Button("Load from File"))
	{
		wayCam->LoadWaypoints("../Resource/Data/Stage1Waypoint.dat");
	}

	ImGui::End();
}
#endif


void Stage1::SetTriggerBox()
{
	auto trigger1 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger1->GetComponent<TransformComponent>()->SetPosition(45.f, 70.f, 130.f);
	trigger1->AddSpawner(SpawnType::Zombie, _vec3(35.f, 80.f, 100.f), _vec3(0.f, 0.f, 0.f));
	trigger1->AddSpawner(SpawnType::Zombie, _vec3(40.f, 80.f, 105.f), _vec3(0.f, 0.f, 0.f));
	trigger1->AddSpawner(SpawnType::Zombie, _vec3(44.f, 80.f, 106.f), _vec3(0.f, 0.f, 0.f));

	auto trigger2 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger2->GetComponent<TransformComponent>()->SetPosition(33.f, 70.f, 96.f);
	trigger2->AddSpawner(SpawnType::Skeleton, _vec3(36.f, 80.f, 100.f));
	trigger2->AddSpawner(SpawnType::Skeleton, _vec3(40.f, 80.f, 110.f));
	trigger2->AddSpawner(SpawnType::Skeleton, _vec3(46.f, 80.f, 115.f));

	auto trigger3 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger3->GetComponent<TransformComponent>()->SetPosition(110.f, 95.f, 38.f);
	trigger3->AddSpawner(SpawnType::Creeper, _vec3(120.f, 105.f, 40.f));
	trigger3->AddSpawner(SpawnType::Creeper, _vec3(125.f, 105.f, 45.f));
	trigger3->AddSpawner(SpawnType::Skeleton, _vec3(130.f, 105.f, 50.f));

	auto trigger4 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger4->GetComponent<TransformComponent>()->SetPosition(165.f, 95.f, 47.f);
	trigger4->AddSpawner(SpawnType::Zombie, _vec3(170.f, 105.f, 50.f));
	trigger4->AddSpawner(SpawnType::JungleZombie, _vec3(180.f, 105.f, 50.f));
	trigger4->AddSpawner(SpawnType::JungleZombie, _vec3(195.f, 105.f, 45.f));
	trigger4->AddSpawner(SpawnType::Zombie, _vec3(155.f, 105.f, 64.f));

	auto trigger5 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger5->GetComponent<TransformComponent>()->SetPosition(200.f, 95.f, 118.f);
	trigger5->AddSpawner(SpawnType::Slime, _vec3(210.f, 105.f, 120.f));
	trigger5->AddSpawner(SpawnType::Zombie, _vec3(220.f, 105.f, 145.f));
	trigger5->AddSpawner(SpawnType::Skeleton, _vec3(206.f, 105.f, 140.f));
	trigger5->AddSpawner(SpawnType::Skeleton, _vec3(200.f, 105.f, 140.f));

	auto trigger6 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger6->GetComponent<TransformComponent>()->SetPosition(192.f, 95.f, 174.f);
	trigger6->AddSpawner(SpawnType::Zombie, _vec3(200.f, 105.f, 170.f));
	trigger6->AddSpawner(SpawnType::Zombie, _vec3(216.f, 105.f, 190.f));
	trigger6->AddSpawner(SpawnType::Zombie, _vec3(226.f, 105.f, 166.f));
	trigger6->AddSpawner(SpawnType::Zombie, _vec3(206.f, 105.f, 172.f));

	auto trigger7 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger7->GetComponent<TransformComponent>()->SetPosition(324.f, 80.f, 100.f);
	trigger7->AddSpawner(SpawnType::Slime, _vec3(350.f, 90.f, 100.f));
	trigger7->AddSpawner(SpawnType::Slime, _vec3(360.f, 90.f, 120.f));
	trigger7->AddSpawner(SpawnType::Creeper, _vec3(350.f, 85.f, 130.f));
	trigger7->AddSpawner(SpawnType::Creeper, _vec3(360.f, 85.f, 137.f));

	auto trigger8 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger8->GetComponent<TransformComponent>()->SetPosition(351.f, 80.f, 156.f);
	trigger8->AddSpawner(SpawnType::Zombie, _vec3(350.f, 90.f, 170.f));
	trigger8->AddSpawner(SpawnType::Zombie, _vec3(353.f, 90.f, 180.f));
	trigger8->AddSpawner(SpawnType::Zombie, _vec3(347.f, 90.f, 200.f));

	auto trigger9 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger9->GetComponent<TransformComponent>()->SetPosition(350.f, 80.f, 237.f);
	trigger9->AddSpawner(SpawnType::Creeper, _vec3(349.f,90.f, 250.f));
	trigger9->AddSpawner(SpawnType::Zombie, _vec3(349.f, 90.f, 280.f));
	trigger9->AddSpawner(SpawnType::Zombie, _vec3(349.f, 90.f, 270.f));

	auto trigger10 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger10->GetComponent<TransformComponent>()->SetPosition(308.f, 70.f, 321.f);
	trigger10->AddSpawner(SpawnType::Slime, _vec3(260.f, 80.f, 310.f));
	trigger10->AddSpawner(SpawnType::Zombie, _vec3(250.f, 80.f, 320.f));
	trigger10->AddSpawner(SpawnType::JungleZombie, _vec3(240.f, 80.f, 315.f));

	auto bossTrigger = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	bossTrigger->GetComponent<TransformComponent>()->SetPosition(140.f, 70.f, 320.f);
	bossTrigger->AddSpawner(SpawnType::RedGolem, _vec3(40.f, 80.f, 320.f), _vec3(0.f, 0.f, 0.f));
	bossTrigger->RegisterCallBack([this]()
		{
			this->ChangeState(Stage1State::BossIntro);
		});

	ObjectMgr->AddObject(ObjectType::Neutral, trigger1);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger2);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger3);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger4);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger5);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger6);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger7);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger8);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger9);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger10);
	ObjectMgr->AddObject(ObjectType::Neutral, bossTrigger);
}

void Stage1::ChangeState(Stage1State state)
{
	switch (state)
	{
	case Stage1::Stage1State::Stage1Intro:	
	{
		currState = Stage1State::Stage1Intro;
		ChunkMgr->SetChunkRange(40);

		stage1IntroDuration = 25.f;
		stage1IntroTimer = 0.f;
		CameraMgr->SetMainCamera(L"Way_Camera");
		auto cam = static_cast<WayPointCam*>(CameraMgr->GetMainCamera()->GetOwner());

		cam->SetDuration(stage1IntroDuration);
		cam->LoadWaypoints("../Resource/Data/Stage1Waypoint.dat");
		cam->Start();
	}break;
	case Stage1::Stage1State::BossIntro:	
	{
		
	}break;
	case Stage1::Stage1State::Play:
	{
		currState = Stage1State::Play;
		ChunkMgr->SetChunkRange(7);

		ObjectMgr->AddObject(ObjectType::BackGroundEffect, Rain::Create(ObjectMgr, ObjectType::BackGroundEffect));
		CameraMgr->SetMainCamera(L"Third_Camera");
	}break;
	default:
		break;
	}
}

void Stage1::Free()
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
