#include "pch.h"
#include "Stage1.h"

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
}

void Stage1::Update(_float dt)
{
}

void Stage1::Late_Update(_float dt)
{
}

void Stage1::Unload()
{
}

void Stage1::Free()
{
}
