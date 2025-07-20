#include "pch.h"
#include "Village.h"
#include "EngineCore.h"
#include "LoadingScene.h"
#include "SceneManager.h"
#include "ChangeScene.h"

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
#include "ExplodeEffect.h"
#include "LaserHead.h"
#include "EnderProjectile.h"
#include "FireBlock.h"

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
    /*-------------------------Create System-----------------------------*/
    {
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
        player = Player::Create(ObjectMgr, ObjectType::Player);
        ObjectMgr->AddObject(ObjectType::Player, player);

        auto tnt = Tnt::Create(ObjectMgr, ObjectType::Item);
        tnt->GetComponent<TransformComponent>()->SetPosition(20.f, 100.f, 20.f);

        auto fCam = FirstCam::Create(ObjectMgr);
        auto tCam = ThirdCam::Create(ObjectMgr);

        CameraMgr->AddCamera(L"First_Camera", fCam);
        CameraMgr->AddCamera(L"Third_Camera", tCam);
        tCam->SetTarget(player);

        CameraMgr->SetMainCamera(L"First_Camera");

		ObjectMgr->AddObject(ObjectType::Camera, fCam);
		ObjectMgr->AddObject(ObjectType::Camera, tCam);
	} 

	/*-------------------------Create Objects----------------------------*/
	{
		auto chunkload = EngineCore::GetInstance()->GetChunkLoader();
		ChunkMgr->SetChunk(chunkload->GetChunks());

		Grid->InsertBlock();

        UILoader loader;
        loader.LoadUI(ObjectMgr);

        ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));
        ObjectMgr->AddObject(ObjectType::BackGroundEffect, Rain::Create(ObjectMgr, ObjectType::BackGroundEffect));

        /*player->GetComponent<TransformComponent>()->SetPosition(30.f, 100.f, 30.f);

        auto trigger1 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
        trigger1->SetTriggerPosition(_vec3(90.f, 5.f, 30.f));
        trigger1->AddSpawner(SpawnType::Zombie, _vec3(100.f, 6.f, 35.f), _vec3(0.f, 0.f, 0.f));
        trigger1->AddSpawner(SpawnType::Zombie, _vec3(100.f, 6.f, 35.f), _vec3(0.f, 0.f, 0.f));
        trigger1->AddSpawner(SpawnType::Skeleton, _vec3(100.f, 6.f, 35.f), _vec3(0.f, 0.f, 0.f));
        trigger1->AddSpawner(SpawnType::Skeleton, _vec3(100.f, 6.f, 35.f), _vec3(0.f, 0.f, 0.f));

        auto trigger2 = SpawnTriggerBox::Create(ObjectMgr, ObjectType::Neutral);
        trigger2->SetTriggerPosition(_vec3(124.f, 5.f, 86.f));
        trigger2->AddSpawner(SpawnType::Skeleton, _vec3(154.f, 30.f, 120.f), _vec3(0.f, 0.f, 0.f));
        trigger2->AddSpawner(SpawnType::Creeper, _vec3(174.f, 30.f, 100.f), _vec3(0.f, 0.f, 0.f));
        trigger2->AddSpawner(SpawnType::Zombie, _vec3(120.f, 30.f, 90.f), _vec3(0.f, 0.f, 0.f));
        trigger2->AddSpawner(SpawnType::RedGolem, _vec3(150.f, 30.f, 200.f), _vec3(0.f, 0.f, 0.f));

        auto slime = Slime::Create(ObjectMgr, ObjectType::Monster);
        slime->GetComponent<TransformComponent>()->SetPosition(50.f, 400.f, 50.f);
        auto purpleSlime = PurpleSlime::Create(ObjectMgr, ObjectType::Monster);
        purpleSlime->GetComponent<TransformComponent>()->SetPosition(50.f, 400.f, 50.f);

        ObjectMgr->AddObject(ObjectType::Monster, slime);
        ObjectMgr->AddObject(ObjectType::Monster, purpleSlime);
        ObjectMgr->AddObject(ObjectType::Neutral, Npc::Create(ObjectMgr, ObjectType::Monster));*/

        auto ender = Ender::Create(ObjectMgr, ObjectType::Monster);
        ender->GetComponent<TransformComponent>()->SetPosition(20.f, 100.f, 20.f);
        ObjectMgr->AddObject(ObjectType::Monster, ender);

        auto enderprojectile = EnderProjectile::Create(ObjectMgr, ObjectType::Projectile);
        enderprojectile->GetComponent<TransformComponent>()->SetPosition(150.f, 100.f, 150.f);
        ObjectMgr->AddObject(ObjectType::Projectile, enderprojectile);
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

void Village::Free()
{
	//Safe_Release(ObjectMgr);
	//Safe_Release(CollisionSys);
	//Safe_Release(PhysicsSys);
	//Safe_Release(CameraMgr);
	//Safe_Release(BlockMgr);
	//Safe_Release(Grid);
	//Safe_Release(uiMgr);
	//Safe_Release(ChunkMgr);

    Scene::Free();
}
