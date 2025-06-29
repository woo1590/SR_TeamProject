#include "pch.h"
#include "TestScene.h"
#include "EngineCore.h"

//system
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "InputSystem.h"

//object
#include "BasicTerrain.h"
#include "TestObject.h"
#include "SkyBox.h"
#include "HPBarFront.h"
#include "Player.h"
#include "HPBarBack.h"

//component
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"

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

	//ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));
	//ObjectMgr->AddObject(ObjectType::Terrain, BasicTerrain::Create(ObjectMgr, ObjectType::Terrain));
	ObjectMgr->AddObject(ObjectType::Player, TestObject::Create(ObjectMgr, ObjectType::Player));

	player = Player::Create(ObjectMgr, ObjectType::Player);
	auto info = player->GetComponent<PlayerInfoComponent>();
	ObjectMgr->AddObject(ObjectType::Player, player);

	ObjectMgr->AddObject(ObjectType::UI, HPBarFront::Create(ObjectMgr, ObjectType::UI, info));
	ObjectMgr->AddObject(ObjectType::UI, HPBarBack::Create(ObjectMgr, ObjectType::UI));

	/*----------------------------------------------------------------------------------------------*/
}

void TestScene::Update(float dt)
{
	const auto& input = EngineCore::GetInstance()->GetInputSystem();

	if (input->IsKeyPressed(KEY::LBUTTON))
	{
		if (player)
		{
			auto info = player->GetComponent<PlayerInfoComponent>();
			if (info)
			{
				int curHp = info->GetCurHp();
				int newHp = max(0, curHp - 10);
				info->SetHp(newHp);
			}
		}
	}
	ObjectMgr->Update(dt);
}

void TestScene::Late_Update(float dt)
{
	ObjectMgr->Late_Update(dt);
}

void TestScene::Unload()
{

}

void TestScene::Free()
{
	Safe_Release(ObjectMgr);
	Scene::Free();
}