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
	resource->LoadTexture(L"../Resource/Texture/Player/playerHead.dds", L"playerHead", TEXTURE::Tex_Cube);
	auto playerHeadMtrl = Material::Create();
	playerHeadMtrl->SetTexture(L"playerHead");
	resource->LoadMaterial(L"playerHead_Mtrl", playerHeadMtrl);

	resource->LoadTexture(L"../Resource/Texture/Player/playerBody.dds", L"playerBody", TEXTURE::Tex_Cube);
	auto playerBodyMtrl = Material::Create();
	playerBodyMtrl->SetTexture(L"playerBody");
	resource->LoadMaterial(L"playerBody_Mtrl", playerBodyMtrl);

	resource->LoadTexture(L"../Resource/Texture/Player/playerRightArm.dds", L"playerRightArm", TEXTURE::Tex_Cube);
	auto playerRightArmMtrl = Material::Create();
	playerRightArmMtrl->SetTexture(L"playerRightArm");
	resource->LoadMaterial(L"playerRightArm_Mtrl", playerRightArmMtrl);

	resource->LoadTexture(L"../Resource/Texture/Player/playerLeftArm.dds", L"playerLeftArm", TEXTURE::Tex_Cube);
	auto playerLeftArmMtrl = Material::Create();
	playerLeftArmMtrl->SetTexture(L"playerLeftArm");
	resource->LoadMaterial(L"playerLeftArm_Mtrl", playerLeftArmMtrl);

	resource->LoadTexture(L"../Resource/Texture/Player/playerRightLeg.dds", L"playerRightLeg", TEXTURE::Tex_Cube);
	auto playerRightLegMtrl = Material::Create();
	playerRightLegMtrl->SetTexture(L"playerRightLeg");
	resource->LoadMaterial(L"playerRightLeg_Mtrl", playerRightLegMtrl);

	resource->LoadTexture(L"../Resource/Texture/Player/playerLeftLeg.dds", L"playerLeftLeg", TEXTURE::Tex_Cube);
	auto playerLeftLegMtrl = Material::Create();
	playerLeftLegMtrl->SetTexture(L"playerLeftLeg");
	resource->LoadMaterial(L"playerLeftLeg_Mtrl", playerLeftLegMtrl);

	return S_OK;
}

HRESULT Loader::load_UIResources() // 제현 UI 전용함수
{
	static const unordered_map<wstring, TEXTURE> uiTexture = {
		{L"Shield",         TEXTURE::Tex_Diffuse},
		{L"Logo",           TEXTURE::Tex_Diffuse},
		{L"hpbar_front",    TEXTURE::Tex_Diffuse},
		{L"hpbar_back",     TEXTURE::Tex_Diffuse},
		{L"loadingscene",   TEXTURE::Tex_Diffuse},
		{L"Cursor",         TEXTURE::Tex_Diffuse},
		{L"InventoryPanel", TEXTURE::Tex_Diffuse},

	};

	// layer 낮을수록 먼저 그림
	static const unordered_map<wstring, int> uiLayers = {
		{L"hpbar_back", -1},
		{L"hpbar_front", 0},
		{L"Shield", 5},
		{L"Logo", 100},
		{L"loadingscene", -100},
		{L"Cursor", 1000},
		{L"InventoryPanel", -100}
	};

	auto rm = EngineCore::GetInstance()->GetResourceManager();
	const wstring basePath = L"../Resource/Jehyun/";

	for (auto const [key, type] : uiTexture)
	{
		rm->LoadTexture(basePath + key + L".png", key, type);

		if (auto it = uiLayers.find(key); it != uiLayers.end())
			rm->RegisterUILayer(key, it->second);
	}

	return S_OK;
}

void Loader::Free()
{
	WaitForSingleObject(H_Thread, INFINITE);

	CloseHandle(H_Thread);

	DeleteCriticalSection(&Crt);
}
