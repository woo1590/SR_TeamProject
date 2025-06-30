#include "pch.h"
#include "Loader.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "StaticMesh.h"
#include "TerrainMesh.h"
#include "CubeMesh.h"
#include "Material.h"

Loader::Loader(LOADID loadId)
	:LoadId(loadId)
{
}

Loader::~Loader()
{
}

Loader* Loader::Create(LOADID loadId)
{
	Loader* Instance = new Loader(loadId);

	if (FAILED(Instance->Ready_Loader()))
	{
		Safe_Release(Instance);

		Instance = nullptr;	
	}

	return Instance;
}

HRESULT Loader::Ready_Loader()
{
	InitializeCriticalSection(&Crt);

	H_Thread = (HANDLE)_beginthreadex(NULL, 0, LoaderMain, this, 0, NULL);

	return S_OK;
}

HRESULT Loader::Loading()
{
	_ulong hr = 0;

	switch (LoadId)
	{
	case LOADID::TestScene:
		hr = Load_TestScene();
		hr = load_UIResources();
		break;
	default:
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	Is_Finish = true;
	return S_OK;
}

_bool Loader::IsFinished() const
{
	return Is_Finish;
}

_uint __stdcall Loader::LoaderMain(void* arg)
{
	Loader* loader = reinterpret_cast<Loader*>(arg);

	_ulong hr = 0;

	EnterCriticalSection(&loader->Crt);
	hr = loader->Loading();
	LeaveCriticalSection(&loader->Crt);

	if (FAILED(hr))
		return -1;

	return 0;
}

HRESULT Loader::Load_TestScene()
{
	auto cube = CubeMesh::Create();
	auto skyboxMtrl = Material::Create();
	auto terrainMtrl = Material::Create();
	auto resource = EngineCore::GetInstance()->GetResourceManager();

	/*--------------Load File Resource-----------------*/
	resource->LoadTerrain(L"../Resource/Texture/Terrain/Height1.bmp", L"Basic_Terrain", 50.f, 1.f);
	resource->LoadTexture(L"../Resource/Texture/Terrain/Terrain0.png",L"Basic_Terrain_Texture", TEXTURE::Tex_Diffuse);
	terrainMtrl->SetTexture(L"Basic_Terrain_Texture");
	resource->LoadMaterial(L"Terrain_Mtrl", terrainMtrl);

	resource->LoadMesh(L"Cube_Mesh", cube);
	resource->LoadTexture(L"../Resource/Texture/SkyBox/burger3.dds", L"Basic_SkyBox", TEXTURE::Tex_Cube);
	skyboxMtrl->SetTexture(L"Basic_SkyBox");
	resource->LoadMaterial(L"SkyBox_Mtrl", skyboxMtrl);

	//////////////////////////////////////////////////////////////////////////////////
	resource->LoadMesh(L"Cube_Mesh", cube);
	resource->LoadTexture(L"../Resource/Asset/HY/Body2.dds", L"Body", TEXTURE::Tex_Cube);
	auto zombieMtrl = Material::Create();
	zombieMtrl->SetTexture(L"Body");
	resource->LoadMaterial(L"ZombieBody_Mtrl", zombieMtrl);

	resource->LoadMesh(L"Cube_Mesh", cube);
	resource->LoadTexture(L"../Resource/Asset/HY/ZombieHead.dds", L"Head", TEXTURE::Tex_Cube);
	zombieMtrl = Material::Create();
	zombieMtrl->SetTexture(L"Head");
	resource->LoadMaterial(L"ZombieHead_Mtrl", zombieMtrl);

	resource->LoadMesh(L"Cube_Mesh", cube);
	resource->LoadTexture(L"../Resource/Asset/HY/Arm.dds", L"Arm", TEXTURE::Tex_Cube);
	zombieMtrl = Material::Create();
	zombieMtrl->SetTexture(L"Arm");
	resource->LoadMaterial(L"ZombieArm_Mtrl", zombieMtrl);

	resource->LoadMesh(L"Cube_Mesh", cube);
	resource->LoadTexture(L"../Resource/Asset/HY/Leg.dds", L"Leg", TEXTURE::Tex_Cube);
	zombieMtrl = Material::Create();
	zombieMtrl->SetTexture(L"Leg");
	resource->LoadMaterial(L"ZombieLeg_Mtrl", zombieMtrl);
	//////////////////////////////////////////////////////////////////////////////////

	resource->LoadTexture(L"../Resource/Texture/Block/DirtBlock.dds", L"DirtBlock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"DirtBlock", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/GrassBlock.dds", L"GrassBlock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"GrassBlock", cube);

	auto dirtBlockMtrl = Material::Create();
	dirtBlockMtrl->SetTexture(L"DirtBlock");
	resource->LoadMaterial(L"DirtBlock_Mtrl", dirtBlockMtrl);

	auto grassBlockMtrl = Material::Create();
	grassBlockMtrl->SetTexture(L"GrassBlock");
	resource->LoadMaterial(L"GrassBlock_Mtrl", grassBlockMtrl);

	return S_OK;
}

HRESULT Loader::load_UIResources()
{
	auto rm = EngineCore::GetInstance()->GetResourceManager();

	rm->LoadTexture(L"Shield", L"../Bin/Assets/Jehyun/Shield.png",TEXTURE::Tex_Diffuse);

	return S_OK;
}

void Loader::Free()
{
	WaitForSingleObject(H_Thread, INFINITE);

	CloseHandle(H_Thread);

	DeleteCriticalSection(&Crt);
}
