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
#include "InventoryCam.h"

//component
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"
#include "SpriteRenderer.h"
#include "ThirdcamComponent.h"
#include "InfoComponent.h"

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
		auto inventoryCam = InventoryCam::Create(ObjectMgr);

		CameraMgr->AddCamera(L"First_Camera", fCam);
		CameraMgr->AddCamera(L"Third_Camera", tCam);
		CameraMgr->AddCamera(L"Inventory_Camera", inventoryCam);
		tCam->SetTarget(player);

		CameraMgr->SetMainCamera(L"Third_Camera");

		ObjectMgr->AddObject(ObjectType::Camera, fCam);
		ObjectMgr->AddObject(ObjectType::Camera, tCam);
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
		ObjectMgr->AddObject(ObjectType::BackGroundEffect, Rain::Create(ObjectMgr, ObjectType::BackGroundEffect));

		player->GetComponent<TransformComponent>()->SetPosition(110.f, 120.f, 170.f);

		SetTriggerBox();
		
	}
}

void Stage1::Update(_float dt)
{
	ObjectMgr->Update(dt);
	PhysicsSys->Update(dt);
	ChunkMgr->IsChunkBoundary(CameraMgr->GetMainCamera()->GetOwner()->GetComponent<TransformComponent>()->GetPosition());
	uiMgr->Update(dt);

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

void Stage1::Late_Update(_float dt)
{
	ObjectMgr->Late_Update(dt);
}

void Stage1::Unload()
{
	EngineCore::GetInstance()->GetSoundManager()->Stop("Stage1BGM");

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
void Stage1::DebugIMGUI()
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
		//camera intro
		break;
	case Stage1::Stage1State::BossIntro:
		boss = RedGolem::Create(ObjectMgr, ObjectType::Monster);
		boss->GetComponent<TransformComponent>()->SetPosition(120.f, 70.f, 100.f);
		ObjectMgr->AddObject(ObjectType::Monster, boss);
		boss->AddRef();
		currState = Stage1State::Play;
		break;
	case Stage1::Stage1State::Play:
		break;
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
