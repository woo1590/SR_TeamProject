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

		ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));

		player->GetComponent<TransformComponent>()->SetPosition(110.f, 120.f, 170.f);

		SetTriggerBox();
		
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

		if (Input->IsKeyPressed(NUM1))
			CameraMgr->SetMainCamera(L"First_Camera");

		if (Input->IsKeyPressed(NUM2))
			CameraMgr->SetMainCamera(L"Third_Camera");

		if (Input->IsKeyPressed(NUM9))
			EngineCore::GetInstance()->SetDebugMode(false);

		if (Input->IsKeyPressed(NUM0))
			EngineCore::GetInstance()->SetDebugMode(true);


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
#endif


void Stage1::SetTriggerBox()
{
	auto trigger1 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	trigger1->GetComponent<TransformComponent>()->SetPosition(45.f, 70.f, 130.f);
	trigger1->AddSpawner(SpawnType::Zombie, _vec3(40.f, 80.f, 104.f), _vec3(0.f, 0.f, 0.f));
	trigger1->AddSpawner(SpawnType::Zombie, _vec3(40.f, 80.f, 104.f), _vec3(0.f, 0.f, 0.f));
	trigger1->AddSpawner(SpawnType::Zombie, _vec3(40.f, 80.f, 104.f), _vec3(0.f, 0.f, 0.f));

	auto trigger2 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	auto trigger3 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	auto trigger4 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	auto trigger5 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);


	auto bossTrigger = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
	bossTrigger->GetComponent<TransformComponent>()->SetPosition(140.f, 70.f, 320.f);
	bossTrigger->AddSpawner(SpawnType::RedGolem, _vec3(40.f, 80.f, 320.f), _vec3(0.f, 0.f, 0.f));

	ObjectMgr->AddObject(ObjectType::Neutral, bossTrigger);
}

void Stage1::ChangeState(Stage1State state)
{
	switch (state)
	{
	case Stage1::Stage1State::Stage1Intro:	
	{
		currState = Stage1State::Stage1Intro;

		stage1IntroDuration = 15.f;
		stage1IntroTimer = 0.f;
		CameraMgr->SetMainCamera(L"Way_Camera");
		auto cam = static_cast<WayPointCam*>(CameraMgr->GetMainCamera()->GetOwner());

		cam->SetDuration(stage1IntroDuration);
		//cam->AddWaypoint({ _vec3(110.f,105.f,216.f),_vec3(-0.3f,-0.7f,-0.7f) });
		//cam->AddWaypoint({ _vec3(60.f,110.f,215.f),_vec3(0.1f,-0.7f,-0.7f) });
		//cam->AddWaypoint({ _vec3(12.f,115.f,181.f),_vec3(0.5f,-0.7f,-0.4f) });
		//cam->AddWaypoint({ _vec3(-17.f,119.f,107.f),_vec3(0.8f,-0.5f,-0.2f) });
		//cam->AddWaypoint({ _vec3(3.f,123.f,62.f),_vec3(0.8f,-0.5f,-0.2f) });//5

		cam->AddWaypoint({ _vec3(37.f,131.f,18.f),_vec3(0.7f,-0.6f,0.5f) });
		cam->AddWaypoint({ _vec3(75.f,140.f,-5.f),_vec3(0.4f,-0.5f,0.7f) });
		cam->AddWaypoint({ _vec3(135.f,144.f,-20.f),_vec3(0.f,-0.4f,0.8f) });
		cam->AddWaypoint({ _vec3(164.f,160.f,11.f),_vec3(0.1f,-0.3f,0.5f) });
		cam->AddWaypoint({ _vec3(156.f,169.f,54.f),_vec3(0.1f,-0.3f,0.3f) });//10

		cam->AddWaypoint({ _vec3(156.f,161.f,11.f),_vec3(0.1f,-0.3f,0.5f) });
		cam->AddWaypoint({ _vec3(150.f,161.f,160.f),_vec3(0.6f,-0.5f,0.5f) });
		cam->AddWaypoint({ _vec3(216.f,161.f,176.f),_vec3(0.1f,-0.4f,0.8f) });
		cam->AddWaypoint({ _vec3(226.f,154.f,204.f),_vec3(0.1f,-0.3f,-0.4f) });
		cam->AddWaypoint({ _vec3(290.f,154.f,146.f),_vec3(-0.2f,-0.6f,0.7f) });//15

		//cam->AddWaypoint({ _vec3(390.f,135.f,125.f),_vec3(-0.5f,-0.5f,0.7f) });
		//cam->AddWaypoint({ _vec3(421.f,135.f,180.f),_vec3(-0.8f,-0.4f,0.3f) });
		//cam->AddWaypoint({ _vec3(427.f,135.f,232.f),_vec3(-0.8f,-0.4f,0.2f) });
		//cam->AddWaypoint({ _vec3(417.f,140.f,314.f),_vec3(-0.8f,-0.5f,-0.4f) });
		//cam->AddWaypoint({ _vec3(363.f,135.f,382.f),_vec3(-0.7f,-0.4f,-0.5f) });//20

		//cam->AddWaypoint({ _vec3(300.f,163.f,386.f),_vec3(-0.4f,-0.7f,-0.5f) });
		//cam->AddWaypoint({ _vec3(232.f,156.f,433.f),_vec3(-0.4f,-0.6f,-0.6f) });
		//cam->AddWaypoint({ _vec3(161.f,166.f,456.f),_vec3(0.f,-0.7f,-0.7f) });
		//cam->AddWaypoint({ _vec3(107.f,168.f,444.f),_vec3(0.1f,-0.7f,-0.7f) });

		cam->Start();
	}break;
	case Stage1::Stage1State::BossIntro:	
	{
		
	}break;
	case Stage1::Stage1State::Play:
	{
		currState = Stage1State::Play;

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
