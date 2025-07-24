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
#include "FixedCam.h"
#include "MiniMapObject.h"
#include "MiniMapRenderer.h"

//component
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"
#include "SpriteRenderer.h"
#include "ThirdcamComponent.h"
#include "InfoComponent.h"


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
		auto fixCam = FixedCam::Create(ObjectMgr);
		fixCam->SetPosition(220.f, 74.f, 200.f);
		fixCam->SetForward(-0.67f, -0.3f, 0.6f);

		auto inventoryCam = InventoryCam::Create(ObjectMgr);

		CameraMgr->AddCamera(L"First_Camera", fCam);
		CameraMgr->AddCamera(L"Third_Camera", tCam);
		CameraMgr->AddCamera(L"Way_Camera", wCam);
		CameraMgr->AddCamera(L"Fix_Camera", fixCam);
		CameraMgr->AddCamera(L"Inventory_Camera", inventoryCam);
		tCam->SetTarget(player);

		CameraMgr->SetMainCamera(L"Third_Camera");

		ObjectMgr->AddObject(ObjectType::Camera, fCam);
		ObjectMgr->AddObject(ObjectType::Camera, tCam);
		ObjectMgr->AddObject(ObjectType::Camera, wCam);
		ObjectMgr->AddObject(ObjectType::Camera, fixCam);
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
			levers.push_back(lever);
			lever->AddRef();
		}
		
		miniMapObject = MiniMapObject::Create(ObjectMgr);
		sceneID = STAGE1;

		for (auto& [pair, chunk] : ChunkMgr->GetChunks())
			ChunkMgr->CreateMiniMapChunk(pair.first, pair.second, chunk, sceneID);


		Grid->InsertBlock();
	}

	/*------------------Create Object---------------*/
	{
		UILoader loader;
		loader.LoadUI(ObjectMgr);

		auto skybox = SkyBox::Create(ObjectMgr, ObjectType::SkyBox);
		skybox->GetComponent<MeshRenderer>()->SetMaterial("Stage2SkyBox_Mtrl");
	
		ObjectMgr->AddObject(ObjectType::SkyBox, skybox);
		player->GetComponent<TransformComponent>()->SetPosition(210.f, 60.f, 90.f);

		SetTriggerBox();
	}

	ChangeState(Stage2Stage::Stage2Intro);
}

void Stage2::Update(_float dt)
{
	ObjectMgr->Update(dt);
	PhysicsSys->Update(dt);
	ChunkMgr->IsChunkBoundary(CameraMgr->GetMainCamera()->GetOwner()->GetComponent<TransformComponent>()->GetPosition());
	uiMgr->Update(dt);

	auto Input = EngineCore::GetInstance()->GetInputSystem();

	if (miniMapObject->GetMiniMapRenderer()->GetVisible())
		miniMapObject->GetMiniMapRenderer()->UpdateMapData(player->GetComponent<TransformComponent>()->GetPosition(),
			ChunkMgr,
			sceneID);

	if (Input->IsKeyPressed(N))
	{
		bool visible = miniMapObject->GetMiniMapRenderer()->GetVisible();
		miniMapObject->GetMiniMapRenderer()->SetVisible(!visible);
	}

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
		if (bridgeActiveTimer >= bridgeActiveDuration)
			ChangeState(Stage2Stage::Play);

		if (bridgeActiveTimer >= 10.5f && !cameraShake)
		{
			CameraMgr->GetMainCamera()->SetShake(10.f, 0.7f);
			cameraShake = true;
		}

		bridgeActiveTimer += dt;

	}break;
	case Stage2::Stage2Stage::Play:
	{
		if (!isBridgeActive)
		{
			int cnt = 0;
			for (const auto& lever : levers)
				if (lever->IsTrigger())
					cnt++;

			if (cnt >= 3)
				ChangeState(Stage2Stage::ActiveBridge);
		}

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
			ChangeState(Stage2Stage::Play);

	}
}

void Stage2::Late_Update(_float dt)
{
	ObjectMgr->Late_Update(dt);
}

void Stage2::Unload()
{
	EngineCore::GetInstance()->GetSoundManager()->Stop("Stage2BGM");
	EngineCore::GetInstance()->GetSoundManager()->Stop("Boss_EnderBGM");

	for (auto& obj : ObjectMgr->GetObjectList(ObjectType::Player))
	{
		if (auto playerComponent = obj->GetComponent<InfoComponent<PlayerInfo>>())
			playerComponent->ClearObservers();
	}
	for (auto& obj : ObjectMgr->GetObjectList(ObjectType::Monster))
	{
		if (auto monsterComponent = obj->GetComponent<InfoComponent<EnemyInfo>>())
			monsterComponent->ClearObservers();
	}
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

void Stage2::SetTriggerBox()
{
	auto trigger1 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger1->GetComponent<CollisionComponent>()->SetSize(_vec3(60.f, 10.f, 10.f));
	trigger1->GetComponent<TransformComponent>()->SetPosition(186.f, 35.f, 115.f);
	trigger1->AddSpawner(SpawnType::Blastling, _vec3(190.f, 50.f, 115.f));
	trigger1->AddSpawner(SpawnType::PurpleSlime, _vec3(210.f, 60.f, 125.f));
	trigger1->AddSpawner(SpawnType::PurpleSlime, _vec3(170.f, 60.f, 122.f));

	auto trigger2 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger2->GetComponent<CollisionComponent>()->SetSize(_vec3(60.f, 10.f, 10.f));
	trigger2->GetComponent<TransformComponent>()->SetPosition(211.f, 35.f, 136.f);
	trigger2->AddSpawner(SpawnType::Shulker, _vec3(220.f, 50.f, 140.f));
	trigger2->AddSpawner(SpawnType::Blastling, _vec3(250.f, 50.f, 130.f));
	trigger2->AddSpawner(SpawnType::Blastling, _vec3(200.f, 50.f, 110.f));

	auto trigger3 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger3->GetComponent<CollisionComponent>()->SetSize(_vec3(60.f, 10.f, 10.f));
	trigger3->GetComponent<TransformComponent>()->SetPosition(257.f, 35.f, 100.f);
	trigger3->AddSpawner(SpawnType::Shulker, _vec3(270.f, 45.f, 120.f));
	trigger3->AddSpawner(SpawnType::Shulker, _vec3(280.f, 45.f, 100.f));
	trigger3->AddSpawner(SpawnType::Blastling, _vec3(230.f, 45.f, 110.f));
	trigger3->AddSpawner(SpawnType::Blastling, _vec3(250.f, 45.f, 90.f));

	auto trigger4 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger4->GetComponent<CollisionComponent>()->SetSize(_vec3(60.f, 10.f, 10.f));
	trigger4->GetComponent<TransformComponent>()->SetPosition(328.f, 35.f, 100.f);
	trigger4->AddSpawner(SpawnType::Blastling, _vec3(270.f, 50.f, 120.f));
	trigger4->AddSpawner(SpawnType::Blastling, _vec3(290.f, 50.f, 100.f));
	trigger4->AddSpawner(SpawnType::PurpleSlime, _vec3(350.f, 50.f, 110.f));
	trigger4->AddSpawner(SpawnType::Blastling, _vec3(340.f, 50.f, 110.f));

	auto trigger5 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger5->GetComponent<CollisionComponent>()->SetSize(_vec3(60.f, 10.f, 10.f));
	trigger5->GetComponent<TransformComponent>()->SetPosition(405.f, 35.f, 120.f);
	trigger5->AddSpawner(SpawnType::PurpleSlime, _vec3(380.f, 50.f, 130.f));
	trigger5->AddSpawner(SpawnType::Shulker, _vec3(380.f, 50.f, 130.f));
	trigger5->AddSpawner(SpawnType::Blastling, _vec3(360.f, 50.f, 160.f));
	trigger5->AddSpawner(SpawnType::PurpleSlime, _vec3(410.f, 50.f, 120.f));

	auto trigger6 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger6->GetComponent<CollisionComponent>()->SetSize(_vec3(60.f, 10.f, 10.f));
	trigger6->GetComponent<TransformComponent>()->SetPosition(437.f, 35.f, 170.f);
	trigger6->AddSpawner(SpawnType::Blastling, _vec3(440.f, 50.f, 175.f));
	trigger6->AddSpawner(SpawnType::Blastling, _vec3(420.f, 50.f, 190.f));
	trigger6->AddSpawner(SpawnType::Blastling, _vec3(410.f, 50.f, 155.f));

	auto trigger7 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger7->GetComponent<CollisionComponent>()->SetSize(_vec3(60.f, 10.f, 10.f));
	trigger7->GetComponent<TransformComponent>()->SetPosition(476.f, 35.f, 193.f);
	trigger7->AddSpawner(SpawnType::Shulker, _vec3(480.f, 50.f, 180.f));
	trigger7->AddSpawner(SpawnType::Shulker, _vec3(450.f, 50.f, 170.f));
	trigger7->AddSpawner(SpawnType::PurpleSlime, _vec3(460.f, 50.f, 180.f));

	auto trigger8 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger8->GetComponent<CollisionComponent>()->SetSize(_vec3(60.f, 10.f, 10.f));
	trigger8->GetComponent<TransformComponent>()->SetPosition(483.f, 35.f, 236.f);
	trigger8->AddSpawner(SpawnType::Blastling, _vec3(480.f, 50.f, 250.f));
	trigger8->AddSpawner(SpawnType::Blastling, _vec3(450.f, 50.f, 220.f));
	trigger8->AddSpawner(SpawnType::Blastling, _vec3(460.f, 50.f, 240.f));

	auto trigger9 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger9->GetComponent<CollisionComponent>()->SetSize(_vec3(60.f, 10.f, 10.f));
	trigger9->GetComponent<TransformComponent>()->SetPosition(457.f, 35.f, 291.f);
	trigger9->AddSpawner(SpawnType::PurpleSlime, _vec3(450.f, 50.f, 280.f));
	trigger9->AddSpawner(SpawnType::PurpleSlime, _vec3(470.f, 50.f, 270.f));
	trigger9->AddSpawner(SpawnType::PurpleSlime, _vec3(460.f, 50.f, 240.f));

	auto trigger10 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger10->GetComponent<TransformComponent>()->SetPosition(471.f, 35.f, 360.f);

	auto bossTrigger = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	bossTrigger->GetComponent<TransformComponent>()->SetPosition(130.f, 40.f, 410.f);
	bossTrigger->AddSpawner(SpawnType::Ender, _vec3(125.f, 70.f, 460.f), _vec3(0.f, 0.f, 0.f));
	bossTrigger->RegisterCallBack([this]()
		{
			this->ChangeState(Stage2Stage::BossIntro);
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

void Stage2::ChangeState(Stage2Stage state)
{
	switch (state)
	{
	case Stage2::Stage2Stage::Stage2Intro:
	{
		currState = Stage2Stage::Stage2Intro;
		ChunkMgr->SetChunkRange(40);

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
		currState = Stage2Stage::BossIntro;

		EngineCore::GetInstance()->GetSoundManager()->Stop("Stage2BGM");
		EngineCore::GetInstance()->GetSoundManager()->PlayBGM("Boss_EnderBGM");

		ChangeState(Stage2Stage::Play);

	}break;
	case Stage2::Stage2Stage::Play:
	{
		currState = Stage2Stage::Play;
		ChunkMgr->SetChunkRange(7);

		CameraMgr->SetMainCamera(L"Third_Camera");

	}break;
	case Stage2::Stage2Stage::ActiveBridge:
	{
		currState = Stage2Stage::ActiveBridge;
		ChunkMgr->SetChunkRange(40);

		bridgeActiveDuration = 13.f;
		bridgeActiveTimer = 0.f;
		isBridgeActive = true;

		CameraMgr->SetMainCamera(L"Fix_Camera");

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
