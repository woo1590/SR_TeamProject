#include "pch.h"
#include "ChangeScene.h"
#include "EngineCore.h"
#include "SceneManager.h"
#include "LoadingScene.h"

ChangeScene::ChangeScene(LOADID nextSceneID)
	:nextSceneID(nextSceneID)
{
}

ChangeScene::~ChangeScene()
{
}

ChangeScene* ChangeScene::Create(LOADID nextSceneID)
{
	ChangeScene* Instance = new ChangeScene(nextSceneID);

	return Instance;
}

void ChangeScene::Execute()
{
	auto nextScene = LoadingScene::Create(nextSceneID);
	EngineCore::GetInstance()->GetSceneManager()->SetActiveScene(nextScene);
}
