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
	auto resource = EngineCore::GetInstance()->GetResourceManager();

	/*--------------Load File Resource-----------------*/
	resource->LoadTerrain(L"../Resource/Texture/Terrain/Height1.bmp", L"Basic_Terrain", 50.f, 1.f);
	resource->LoadTexture(L"../Resource/Texture/Terrain/Terrain0.png",L"Basic_Terrain_Texture", TEXTURE::Tex_Diffuse);
	auto terrainMtrl = Material::Create();
	terrainMtrl->SetTexture(L"Basic_Terrain_Texture");
	resource->LoadMaterial(L"Terrain_Mtrl", terrainMtrl);

	auto cube = CubeMesh::Create();
	resource->LoadMesh(L"Cube_Mesh", cube);
	resource->LoadTexture(L"../Resource/Texture/SkyBox/burger3.dds", L"Basic_SkyBox", TEXTURE::Tex_Cube);
	auto skyboxMtrl = Material::Create();
	skyboxMtrl->SetTexture(L"Basic_SkyBox");
	resource->LoadMaterial(L"SkyBox_Mtrl", skyboxMtrl);

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
