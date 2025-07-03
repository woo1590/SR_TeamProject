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
	resource->LoadMesh(L"Cube_Mesh", cube);
	resource->LoadResource(L"../Resource/Texture/SkyBox/burger3.dds", L"Basic_SkyBox", TEXTURE::Tex_Cube, L"SkyBox_Mtrl");
	
	//////////////////////////////////////////////////////////////////////////////////
	resource->LoadResource(L"../Resource/Asset/HY/Body2.dds", L"Body", TEXTURE::Tex_Cube, L"ZombieBody_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/ZombieHead.dds", L"Head", TEXTURE::Tex_Cube, L"ZombieHead_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/Arm.dds", L"Arm", TEXTURE::Tex_Cube, L"ZombieArm_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/Leg.dds", L"Leg", TEXTURE::Tex_Cube, L"ZombieLeg_Mtrl");
	
	//////////////////////////////////////////////////////////////////////////////////
	resource->LoadResource(L"../Resource/Texture/Player/playerHead.dds", L"playerHead", TEXTURE::Tex_Cube, L"playerHead_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Player/playerBody.dds", L"playerBody", TEXTURE::Tex_Cube, L"playerBody_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Player/playerRightArm.dds", L"playerRightArm", TEXTURE::Tex_Cube, L"playerRightArm_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Player/playerLeftArm.dds", L"playerLeftArm", TEXTURE::Tex_Cube, L"playerLeftArm_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Player/playerRightLeg.dds", L"playerRightLeg", TEXTURE::Tex_Cube, L"playerRightLeg_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Player/playerLeftLeg.dds", L"playerLeftLef", TEXTURE::Tex_Cube, L"playerLeftLeg_Mtrl");

	/////////////////////////////////////////////////////////////////////////////////
	resource->LoadResource(L"../Resource/Asset/HY/Skeleton_Body.dds", L"SkeletonBody", TEXTURE::Tex_Cube, L"SkeletonBody_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/Skeleton_Face.dds", L"SkeletonFace", TEXTURE::Tex_Cube, L"SkeletonFace_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/Skeleton_Bone.dds", L"SkeletonBone", TEXTURE::Tex_Cube, L"SkeletonBone_Mtrl");

	resource->LoadTexture(L"../Resource/Texture/Block/DirtBlock.dds", L"DirtBlock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"DirtBlock", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/GrassBlock.dds", L"GrassBlock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"GrassBlock", cube);
	
	resource->LoadTexture(L"../Resource/Texture/Block/WoodBlock.dds", L"WoodBlock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"WoodBlock", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/WoodPlank.dds", L"WoodPlank", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"WoodPlank", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/WoodBlock.dds", L"WoodBlock", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"WoodBlock", cube);

	resource->LoadTexture(L"../Resource/Texture/Block/WoodPlank.dds", L"WoodPlank", TEXTURE::Tex_Cube);
	resource->LoadMesh(L"WoodPlank", cube);

	auto dirtBlockMtrl = Material::Create();
	dirtBlockMtrl->SetTexture(L"DirtBlock");
	resource->LoadMaterial(L"DirtBlock_Mtrl", dirtBlockMtrl);

	auto grassBlockMtrl = Material::Create();
	grassBlockMtrl->SetTexture(L"GrassBlock");
	resource->LoadMaterial(L"GrassBlock_Mtrl", grassBlockMtrl);

	auto woodBlockMtrl = Material::Create();
	woodBlockMtrl->SetTexture(L"WoodBlock");
	resource->LoadMaterial(L"WoodBlock_Mtrl", woodBlockMtrl);

	auto woodPlankMtrl = Material::Create();
	woodPlankMtrl->SetTexture(L"WoodPlank");
	resource->LoadMaterial(L"WoodPlank_Mtrl", woodPlankMtrl);

	return S_OK;
}

HRESULT Loader::load_UIResources()
{
	const auto eUI = TEXTURE::Tex_Diffuse;

	static const unordered_map<wstring, TEXTURE> uiTexture = {
		{L"Shield",             eUI},
		{L"Logo",               eUI},
		{L"hpbar_front",        eUI},
		{L"hpbar_back",         eUI},
		{L"loadingscene",       eUI},
		{L"Cursor",             eUI},
		{L"InventoryPanel",     eUI},
		{L"hotbar_back",        eUI},
		{L"expbar_front",       eUI},
		{L"expbar_back",        eUI},
		{L"quickslot",          eUI},
		{L"icon_emerald",       eUI},
		{L"expbar_back",        eUI},
		{L"arrow_slot",         eUI},
		{L"quickslot_highlight",eUI},
		{L"inventorybtn",       eUI},
		{L"mapbtn",             eUI},
		{L"mouse_left",         eUI},
		{L"mouse_right",        eUI},
		{L"dash_icon",          eUI},
		{L"quickslot_plus",     eUI},
		{L"exitbtn",            eUI},
		{L"exitbtn_hover",      eUI},

	};

	static const unordered_map<wstring, int> uiLayers = {
		{L"Logo",               -1},
		{L"loadingscene",       -1},
		{L"InventoryPanel",     -1},
		{L"hotbar_back",         0},
		{L"hpbar_back",          1},
		{L"expbar_back",         1},
		{L"expbar_back",         1},
		{L"icon_emerald",        1},
		{L"expbar_front",        2},
		{L"quickslot",           2},
		{L"hpbar_front",         2},
		{L"quickslot_highlight", 2},
		{L"arrow_slot",          2},
		{L"mouse_left",          3},
		{L"mouse_right",         3},
		{L"dash_icon",           3},
		{L"quickslot_plus",      3},
		{L"Shield",              5},
		{L"inventorybtn",        5},
		{L"mapbtn",              5},
		{L"exitbtn",             5},
		{L"exitbtn_hover",       5},
		{L"Cursor",             10},
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
