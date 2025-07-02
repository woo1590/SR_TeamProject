#include "pch.h"
#include "EditScene.h"
#include "EngineCore.h"

//system
#include "ObjectManager.h"
#include "RenderSystem.h"
#include "ResourceManager.h"

//object

//component
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RendererComponent.h"

EditScene::EditScene()
{
}

EditScene::~EditScene()
{
}

EditScene* EditScene::Create()
{
	EditScene* Instance = new EditScene;

	return Instance;
}

void EditScene::Load()
{
	ObjectMgr = ObjectManager::Create(this);

	//ObjectMgr->AddObject(ObjectType::SkyBox, SkyBox::Create(ObjectMgr, ObjectType::SkyBox));

	/*----------------------------------------------------------------------------------------------*/
}

void EditScene::Update(float dt)
{
	ObjectMgr->Update(dt);
}

void EditScene::Late_Update(float dt)
{
	ObjectMgr->Late_Update(dt);
}

void EditScene::Unload()
{

}

void EditScene::Free()
{
	Safe_Release(ObjectMgr);
	Scene::Free();
}