#include "pch.h"
#include "Village.h"
#include "EngineCore.h"
#include "LoadingScene.h"
#include "SceneManager.h"
#include "ChangeScene.h"

//system
#include "GameManager.h"
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
#include "ExplodeEffect.h"
#include "LaserHead.h"
#include "EnderProjectile.h"
#include "FireBlock.h"
#include "NPCdolbok.h"
#include "NPCShop.h"
#include "InventoryCam.h"
#include "Shulker.h"
#include "Blastling.h"
#include "WayPointCam.h"
#include "MiniMapObject.h"
#include "MiniMapRenderer.h"
#include "QuestSystem.h"
#include "CheckPoint.h"
#include "Overlay.h"

//component
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"
#include "SpriteRenderer.h"
#include "ThirdcamComponent.h"
#include "InfoComponent.h"
#include "ProgressBar.h"


Village::Village()
    :Scene()
{
}

Village::~Village()
{
}

Village* Village::Create()
{
    Village* Instance = new Village;

    return Instance;
}

void Village::Load()
{
	auto game = GameManager::GetInstance();

    /*-------------------------Create System-----------------------------*/
    {
#ifdef USE_IMGUI
        EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"Debug", [this]() {this->DebugIMGUI();});
        EngineCore::GetInstance()->GetImGuiManager()->RegisterWindow(L"waY", [this]() {this->WaypointEdit();});
#endif
		EngineCore::GetInstance()->GetSoundManager()->Stop("IntroBGM");
        EngineCore::GetInstance()->GetSoundManager()->PlayBGM("VillageBGM");

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
	} 

	/*-------------------------Create Objects----------------------------*/
	{
		auto chunkload = EngineCore::GetInstance()->GetChunkLoader();
		ChunkMgr->SetChunk(chunkload->GetChunks());
		for (auto& [pair, chunk] : ChunkMgr->GetChunks()) chunk->BuildChunkFace();
		BlockMgr->LoadDB("VillageMap");

		for (auto& dynamic : ObjectMgr->GetObjectList(ObjectType::DynamicBlock))
		{
			auto lever = static_cast<DynamicBlock*>(dynamic);
			if (lever->GetType() != DynamicBlockType::LeverSwitch && lever->GetType() != DynamicBlockType::BasicChest) continue;
			lever->SetTarget(player);
			levers.push_back(lever);
			lever->AddRef();
		}

		miniMapObject = MiniMapObject::Create(ObjectMgr);
		sceneID = TUTORIAL;

		for (auto& [pair, chunk] : ChunkMgr->GetChunks())
			ChunkMgr->CreateMiniMapChunk(pair.first, pair.second, chunk, sceneID);

		Grid->InsertBlock();

        UILoader loader;
        loader.LoadUI(ObjectMgr);

        ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));

		auto npc2 = Npc::Create(ObjectMgr, ObjectType::Neutral);
		npc2->GetComponent<TransformComponent>()->SetPosition(140.f, 10.f, 215.f);
		npc2->GetComponent<TransformComponent>()->SetForward({ 0, 0, -1.f });
		ObjectMgr->AddObject(ObjectType::Neutral, npc2);

		if (game->IsSceneClear(LOADID::Village))
		{
			player->GetComponent<TransformComponent>()->SetPosition(160.f, 10.f, 200.f);
			overlay = Overlay::Create(ObjectMgr, ObjectType::Overlay);
			overlay->SetDuration(1.f);
			overlay->SetFadeIn(true);
			ObjectMgr->AddObject(ObjectType::Overlay, overlay);
		}
		else
		{
			player->GetComponent<TransformComponent>()->SetPosition(278.f, 10.f, 50.f);

			player->SetSpawnPointFromTrigger(LOADID::Village, 0);
			
			auto npc = Npc::Create(ObjectMgr, ObjectType::Neutral);
			npc->GetComponent<TransformComponent>()->SetPosition(270.f, 10.f, 43.f);
			ObjectMgr->AddObject(ObjectType::Neutral, npc);

			SetTriggerBox();
		}
    }

	ChangeState(VillageState::Play);
}

void Village::Update(_float dt)
{
	ObjectMgr->Update(dt);
	PhysicsSys->Update(dt);
	ChunkMgr->IsChunkBoundary(CameraMgr->GetMainCamera()->GetOwner()->GetComponent<TransformComponent>()->GetPosition());
	uiMgr->Update(dt);

	auto Input = EngineCore::GetInstance()->GetInputSystem();

	if (miniMapObject->GetMiniMapRenderer()->GetVisible())
		miniMapObject->GetMiniMapRenderer()->UpdateMapData(ObjectMgr, ChunkMgr, sceneID);

	if (Input->IsKeyPressed(N))
	{
		bool visible = miniMapObject->GetMiniMapRenderer()->GetVisible();
		miniMapObject->GetMiniMapRenderer()->SetVisible(!visible);
	}

	switch (currState)
	{
	case Village::VillageState::EnterVillage:
	{
		if (introTimer >= introDuration)
		{
			ChangeState(VillageState::Play);
			
			uiMgr->GetQuestSystem()->ReportQuestProgress(QuestType::ReachVillage, 1);
		}

		introTimer += dt;
	}break;
	case Village::VillageState::Play:
	{

	}break;
	default:
		break;
	}

    {
		if (Input->IsKeyPressed(NUM9))
			EngineCore::GetInstance()->SetDebugMode(false);
        
        if (Input->IsKeyPressed(NUM0))
			EngineCore::GetInstance()->SetDebugMode(true);
    }
}

void Village::Late_Update(_float dt)
{
    ObjectMgr->Late_Update(dt);

}

void Village::Unload()
{
	GetUIManager()->GetQuestSystem()->SaveDataTo();
	EngineCore::GetInstance()->GetSoundManager()->Stop("VillageBGM");

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
void Village::DebugIMGUI()
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
void Village::WaypointEdit()
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
		wayCam->SaveWaypoints("../Resource/Data/VillageWaypoint.dat");
	}
	ImGui::SameLine();
	if (ImGui::Button("Load from File"))
	{
		wayCam->LoadWaypoints("../Resource/Data/VillageWaypoint.dat");
	}

	ImGui::End();
}
#endif

void Village::SetTriggerBox()
{
	auto trigger1 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger1->GetComponent<TransformComponent>()->SetPosition(238.f, 0.f, 85.f);
	trigger1->AddSpawner(SpawnType::Zombie, _vec3(208.f, 10.f, 116.f), _vec3(0.f, 0.f, 0.f));
	trigger1->AddSpawner(SpawnType::Zombie, _vec3(205.f, 40.f, 85.f), _vec3(0.f, 0.f, 0.f));
	trigger1->AddSpawner(SpawnType::Skeleton, _vec3(185.f, 10.f, 120.f), _vec3(0.f, 0.f, 0.f));

	auto trigger2 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger2->GetComponent<TransformComponent>()->SetPosition(156.f, 0.f, 116.f);
	trigger2->AddSpawner(SpawnType::JungleZombie, _vec3(110.f, 10.f, 70.f), _vec3(0.f, 0.f, 0.f));
	trigger2->AddSpawner(SpawnType::Zombie, _vec3(103.f, 10.f, 40.f), _vec3(0.f, 0.f, 0.f));
	trigger2->AddSpawner(SpawnType::Creeper, _vec3(98.f, 40.f, 81.f), _vec3(0.f, 0.f, 0.f));
	trigger2->AddSpawner(SpawnType::Skeleton, _vec3(70.f, 10.f, 60.f), _vec3(0.f, 0.f, 0.f));

	auto trigger3 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger3->GetComponent<TransformComponent>()->SetPosition(47.f, 0.f, 133.f);
	trigger3->AddSpawner(SpawnType::Slime, _vec3(65.f, 10.f, 200.f), _vec3(0.f, 0.f, 0.f));
	trigger3->AddSpawner(SpawnType::Zombie, _vec3(100.f, 10.f, 200.f), _vec3(0.f, 0.f, 0.f));
	trigger3->AddSpawner(SpawnType::Skeleton, _vec3(71.f, 30.f, 220.f), _vec3(0.f, 0.f, 0.f));
	trigger3->AddSpawner(SpawnType::Skeleton, _vec3(65.f, 10.f, 200.f), _vec3(0.f, 0.f, 0.f));

	auto trigger4 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger4->GetComponent<TransformComponent>()->SetPosition(110.f, 0.f, 200.f);
	trigger4->RegisterCallBack([this]()
		{
			this->ChangeState(VillageState::EnterVillage);
		});

	ObjectMgr->AddObject(ObjectType::Neutral, trigger1);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger2);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger3);
	ObjectMgr->AddObject(ObjectType::Neutral, trigger4);
}

void Village::Free()
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

void Village::ChangeState(VillageState state)
{
	switch (state)
	{
	case Village::VillageState::EnterVillage:
	{
		currState = VillageState::EnterVillage;
		ChunkMgr->SetChunkRange(40);

		introDuration = 10.f;
		introTimer = 0.f;
		CameraMgr->SetMainCamera(L"Way_Camera");
		auto cam = static_cast<WayPointCam*>(CameraMgr->GetMainCamera()->GetOwner());

		cam->SetDuration(introDuration);
		cam->LoadWaypoints("../Resource/Data/VillageWaypoint.dat");
		cam->Start();

	}break;
	case Village::VillageState::Play:
	{
		currState = VillageState::Play;
		CameraMgr->SetMainCamera(L"Third_Camera");

	}break;
	default:
		break;
	}
}
