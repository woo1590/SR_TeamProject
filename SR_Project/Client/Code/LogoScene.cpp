#include "pch.h"
#include "LogoScene.h"

#include "LoadingUI.h"

#include "ObjectManager.h"

LogoScene::LogoScene()
{
}

LogoScene::~LogoScene()
{
}

LogoScene* LogoScene::Create()
{
	return new LogoScene();
}

void LogoScene::Load()
{
	ObjectMgr = ObjectManager::Create(this);

	ObjectMgr->AddObject(ObjectType::UI, LoadingUI::Create(ObjectMgr, ObjectType::UI));
}

void LogoScene::Update(float dt)
{
	ObjectMgr->Update(dt);
}

void LogoScene::Late_Update(float dt)
{
	ObjectMgr->Late_Update(dt);
}

void LogoScene::Unload()
{
}

void LogoScene::Free()
{
	Safe_Release(ObjectMgr);
	Scene::Free();
}
