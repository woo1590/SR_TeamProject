#include "pch.h"
#include "Loader.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "ChunkLoader.h"
#include "SoundManager.h"
#include "StaticMesh.h"
#include "TerrainMesh.h"
#include "CubeMesh.h"
#include "Material.h"
#include "ArrowMesh.h"
#include "QuadMesh.h"

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
	case LOADID::Village:
		hr = Load_Village();
		break;
	case LOADID::Stage1:
		hr = Load_Stage1();
		break;
	case LOADID::Stage2:
		hr = Load_Stage2();
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


HRESULT Loader::Load_Village()
{
	auto chunkload = EngineCore::GetInstance()->GetChunkLoader();

	/*--------------------Load Chunk----------------------*/
	{
		chunkload->LoadChunk(L"../../Reference/MapData/VillageMap.chunkmap");
	}

	return S_OK;
}

HRESULT Loader::Load_Stage1()
{
	auto chunkload = EngineCore::GetInstance()->GetChunkLoader();

	/*--------------------Load Chunk----------------------*/
	{
		chunkload->LoadChunk(L"../../Reference/MapData/Stage1.chunkmap");
	}

	return S_OK;
}

HRESULT Loader::Load_Stage2()
{
	auto chunkload = EngineCore::GetInstance()->GetChunkLoader();

	/*--------------------Load Chunk----------------------*/
	{
		chunkload->LoadChunk(L"../../Reference/MapData/Stage2.chunkmap");
		
	}

	return S_OK;
}

HRESULT Loader::load_UIResources()
{
	
	return S_OK;
}

void Loader::Free()
{
	WaitForSingleObject(H_Thread, INFINITE);

	CloseHandle(H_Thread);

	DeleteCriticalSection(&Crt);
}
