#include "pch.h"
#include "LogoScene.h"

#include "ObjectManager.h"

LogoScene::LogoScene()
{
}

LogoScene::~LogoScene()
{
}

void LogoScene::Load()
{
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
