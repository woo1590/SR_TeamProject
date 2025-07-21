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
#include "Blastling.h"

//component
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"
#include "SpriteRenderer.h"
#include "ThirdcamComponent.h"


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
#endif
        EngineCore::GetInstance()->GetSoundManager()->PlayBGM("TestBGM");

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

	/*-------------------------Create Objects----------------------------*/
	{
		auto chunkload = EngineCore::GetInstance()->GetChunkLoader();
		ChunkMgr->SetChunk(chunkload->GetChunks());

		BlockMgr->LoadDB("VillageMap");

		Grid->InsertBlock();

        UILoader loader;
        loader.LoadUI(ObjectMgr);

        ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));

		if (game->IsSceneClear(LOADID::Village))
		{
			player->GetComponent<TransformComponent>()->SetPosition(160.f, 20.f, 200.f);
		}
		else
		{
			player->GetComponent<TransformComponent>()->SetPosition(260.f, 30.f, 57.f);
			auto npc = Npc::Create(ObjectMgr, ObjectType::Neutral);
			npc->GetComponent<TransformComponent>()->SetPosition(270.f, 20.f, 43.f);
			ObjectMgr->AddObject(ObjectType::Neutral, npc);

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
			trigger3->AddSpawner(SpawnType::Skeleton, _vec3(95.f, 10.f, 220.f), _vec3(0.f, 0.f, 0.f));

			ObjectMgr->AddObject(ObjectType::Neutral, trigger1);
			ObjectMgr->AddObject(ObjectType::Neutral, trigger2);
			ObjectMgr->AddObject(ObjectType::Neutral, trigger3);

			auto blast = Blastling::Create(ObjectMgr, ObjectType::Monster);
			blast->GetComponent<TransformComponent>()->SetPosition(260.f, 30.f, 57.f);
			ObjectMgr->AddObject(ObjectType::Monster, blast);
		}
    }
}

void Village::Update(_float dt)
{
	ObjectMgr->Update(dt);
	PhysicsSys->Update(dt);
	ChunkMgr->IsChunkBoundary(CameraMgr->GetMainCamera()->GetOwner()->GetComponent<TransformComponent>()->GetPosition());
	uiMgr->Update(dt);

    {
        auto Input = EngineCore::GetInstance()->GetInputSystem();

        if (Input->IsKeyPressed(NUM1))
            CameraMgr->SetMainCamera(L"First_Camera");
        
        if (Input->IsKeyPressed(NUM2))
            CameraMgr->SetMainCamera(L"Third_Camera");

		if (Input->IsKeyPressed(NUM4))
		{
            auto command = ChangeScene::Create(LOADID::Stage1);
            EngineCore::GetInstance()->RegisterCommand(command);
			GameManager::GetInstance()->ClearScene(LOADID::Village);
		}

		if (Input->IsKeyPressed(NUM9))
			EngineCore::GetInstance()->SetDebugMode(false);
        
        if (Input->IsKeyPressed(NUM0))
			EngineCore::GetInstance()->SetDebugMode(true);

        if (Input->IsKeyPressed(NUM0))
			EngineCore::GetInstance()->SetDebugMode(true);

		if (Input->IsKeyPressed(NUM3))
		{
			auto fire = FireBlock::Create(ObjectMgr, ObjectType::ParticleEffect);
			fire->GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetPosition());
			fire->SetColor(_vec3(0.8f, 0.5f, 0.8f));
			//fire->SetColor(_vec3(0.3f, 0.3f, 0.3f));
			ObjectMgr->AddObject(ObjectType::ParticleEffect, fire);
		}

		if (Input->IsKeyPressed(Q))
		{
			auto effect = ExplodeEffect::Create(ObjectMgr, ObjectType::ParticleEffect);
			effect->GetComponent<TransformComponent>()->SetPosition(player->GetComponent<TransformComponent>()->GetPosition());
			effect->SetDeadTime(1.f);
			ObjectMgr->AddObject(ObjectType::ParticleEffect, effect);
		}
    }
}

void Village::Late_Update(_float dt)
{
    ObjectMgr->Late_Update(dt);

}

void Village::Unload()
{
	EngineCore::GetInstance()->GetSoundManager()->Stop("TestBGM");
}

#ifdef USE_IMGUI
void Village::DebugIMGUI()
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
