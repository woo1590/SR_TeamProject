#include "pch.h"
#include "Village.h"

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
}

void Village::Update(_float dt)
{
}

void Village::Late_Update(_float dt)
{
}

void Village::Unload()
{
}

void Village::Free()
{
}
