#include "pch.h"
#include "Stage2.h"

Stage2::Stage2()
	:Scene()
{
}

Stage2::~Stage2()
{
}

Stage2* Stage2::Create()
{
	Stage2* Instance = new Stage2;

	return Instance;
}

void Stage2::Load()
{
}

void Stage2::Update(_float dt)
{
}

void Stage2::Late_Update(_float dt)
{
}

void Stage2::Unload()
{
}

void Stage2::Free()
{
}
