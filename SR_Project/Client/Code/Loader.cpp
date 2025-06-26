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

	/*-----------Load Default Resource--------------*/
	std::vector<VTXNOR> vertices;
	std::vector<INDEX32> indices;

	//¾Õ¸é
	vertices.push_back(VTXNOR({ -1.f,-1.f,-1.f }));
	vertices.push_back(VTXNOR({ -1.f,1.f,-1.f }));
	vertices.push_back(VTXNOR({ 1.f,1.f,-1.f }));
	vertices.push_back(VTXNOR({ 1.f,-1.f,-1.f }));

	vertices.push_back(VTXNOR({ -1.f,-1.f,1.f }));
	vertices.push_back(VTXNOR({ -1.f,1.f,1.f }));
	vertices.push_back(VTXNOR({ 1.f,1.f,1.f }));
	vertices.push_back(VTXNOR({ 1.f,-1.f,1.f }));

	//¾Õ¸é
	indices.push_back(INDEX32(0, 1, 2));
	indices.push_back(INDEX32(0, 2, 3));

	//µÞ¸é
	indices.push_back(INDEX32(5, 4, 7));
	indices.push_back(INDEX32(5, 7, 6));

	//¿ÞÂÊ
	indices.push_back(INDEX32(4, 5, 1));
	indices.push_back(INDEX32(4, 1, 0));

	//¿À¸¥ÂÊ
	indices.push_back(INDEX32(3, 2, 6));
	indices.push_back(INDEX32(3, 6, 7));

	//À­¸é
	indices.push_back(INDEX32(1, 5, 6));
	indices.push_back(INDEX32(1, 6, 2));

	//¾Æ·¡¸é
	indices.push_back(INDEX32(4, 0, 3));
	indices.push_back(INDEX32(4, 3, 7));

	auto mesh = StaticMesh::Create();
	mesh->SetMeshData(vertices, indices);

	auto material = Material::Create();

	auto material2 = Material::Create();

	D3DMATERIAL9 blackMtrl;
	blackMtrl.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	blackMtrl.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	blackMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	blackMtrl.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	blackMtrl.Power = 10.f;

	material2->SetMaterial(blackMtrl);

	resource->LoadMesh(L"Default_Cube", mesh);
	resource->LoadMaterial(L"Default_Material", material);
	resource->LoadMaterial(L"DarkGray_Material", material2);

	/*--------------Load File Resource-----------------*/
	auto terrainMtrl = Material::Create();
	terrainMtrl->SetTexture(L"../Resource/Texture/Terrain/Terrain0.png", TEXTURE::Tex_Normal);

	resource->LoadTerrain(L"../Resource/Texture/Terrain/Height1.bmp", L"Basic_Terrain", 50.f, 1.f);
	resource->LoadMaterial(L"Basic_Terrain_Material", terrainMtrl);

	std::vector<VTXCUBE> vertices2;
	
	vertices2.push_back(VTXCUBE({ -1.f,-1.f,-1.f }));
	vertices2.push_back(VTXCUBE({ -1.f,1.f,-1.f }));
	vertices2.push_back(VTXCUBE({ 1.f,1.f,-1.f }));
	vertices2.push_back(VTXCUBE({ 1.f,-1.f,-1.f }));

	vertices2.push_back(VTXCUBE({ -1.f,-1.f,1.f }));
	vertices2.push_back(VTXCUBE({ -1.f,1.f,1.f }));
	vertices2.push_back(VTXCUBE({ 1.f,1.f,1.f }));
	vertices2.push_back(VTXCUBE({ 1.f,-1.f,1.f }));

	auto skyboxMtrl = Material::Create();
	skyboxMtrl->SetTexture(L"../Resource/Texture/SkyBox/burger3.dds", TEXTURE::Tex_Cube);
	auto skyboxMesh = CubeMesh::Create();
	skyboxMesh->SetMeshData(vertices2, indices);

	resource->LoadMaterial(L"SkyBox_Material", skyboxMtrl);
	resource->LoadMesh(L"SkyBox_Mesh", skyboxMesh);
	return S_OK;
}

void Loader::Free()
{
	WaitForSingleObject(H_Thread, INFINITE);

	CloseHandle(H_Thread);

	DeleteCriticalSection(&Crt);
}
