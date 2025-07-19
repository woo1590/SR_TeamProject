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
	auto resource = EngineCore::GetInstance()->GetResourceManager();
	auto sound = EngineCore::GetInstance()->GetSoundManager();
	auto chunkload = EngineCore::GetInstance()->GetChunkLoader();

	/*----------------------Load Mesh----------------------------*/
	{
		auto cube = CubeMesh::Create();
		resource->LoadMesh("Cube_Mesh", cube);
		auto arrow = ArrowMesh::Create();
		resource->LoadMesh("Arrow_Mesh", arrow);
		auto quad = QuadMesh::Create(10);
		resource->LoadMesh("Quad_Mesh", quad);
	}
	/*--------------------Load Shader---------------------------------*/
	{
		resource->LoadShader("../Resource/Shader/SkyBox.fx", "SkyBoxShader");
		resource->LoadShader("../Resource/Shader/BasicShader.fx", "BasicShader");
		resource->LoadShader("../Resource/Shader/ChunkShader.fx", "ChunkShader");
		resource->LoadShader("../Resource/Shader/ColorShader.fx", "ColorShader");
		resource->LoadShader("../Resource/Shader/MaskShader.fx", "MaskShader");
		resource->LoadShader("../Resource/Shader/ParticleShader.fx", "ParticleShader");
		resource->LoadShader("../Resource/Shader/BloodParticleShader.fx", "BloodParticleShader");
		resource->LoadShader("../Resource/Shader/SpriteShader.fx", "SpriteShader");
		resource->LoadShader("../Resource/Shader/ExplodeParticle.fx", "ExplodeParticle");
		resource->LoadShader("../Resource/Shader/LaserShader.fx", "LaserShader");
		resource->LoadShader("../Resource/Shader/FireBlock.fx", "FireBlockShader");
	}

	/*---------------------Load Material--------------------------*/
	{
		resource->LoadMaterial("../Resource/Material/SkyBox.json");
		resource->LoadMaterial("../Resource/Material/Monsters.json");
		resource->LoadMaterial("../Resource/Material/Player.json");
		resource->LoadMaterial("../Resource/Material/Blocks.json");
		resource->LoadMaterial("../Resource/Material/Items.json");
		resource->LoadMaterial("../Resource/Material/Particle.json");
		resource->LoadMaterial("../Resource/Material/Armor.json");
	}

	/*----------------------Load Sound-----------------------------*/
	{
		//BGM
		sound->LoadSound("TestBGM", "../Resource/Sound/BGM/Minecraft.mp3", true);

		//Player
		sound->LoadSound("ShootArrow", "../Resource/Sound/SFX/ShootArrow.mp3", false);
		sound->LoadSound("HitArrow", "../Resource/Sound/SFX/HitArrow.mp3", false);
		sound->LoadSound("ShootFirework", "../Resource/Sound/SFX/ShootFirework.mp3", false);
		sound->LoadSound("BoomFirework", "../Resource/Sound/SFX/BoomFirework.mp3", false);
		sound->LoadSound("SwingSword", "../Resource/Sound/SFX/SwingSword.mp3", false);
		sound->LoadSound("HitSword", "../Resource/Sound/SFX/HitSword.mp3", false);
		sound->LoadSound("Roll", "../Resource/Sound/SFX/Roll.mp3", false);
		sound->LoadSound("BoomTNT", "../Resource/Sound/SFX/BoomTNT.mp3", false);
		sound->LoadSound("DeathPlayer", "../Resource/Sound/SFX/DeathPlayer.mp3", false);
		sound->LoadSound("RevivePlayer", "../Resource/Sound/SFX/RevivePlayer.mp3", false);
		sound->LoadSound("WalkOnDefault1", "../Resource/Sound/SFX/WalkOnDefault1.mp3", false);
		sound->LoadSound("WalkOnDefault2", "../Resource/Sound/SFX/WalkOnDefault2.mp3", false);
		//sound->LoadSound("WalkOnGrass1", "../Resource/Sound/SFX/WalkOnGrass1.mp3", false);
		//sound->LoadSound("WalkOnGrass2", "../Resource/Sound/SFX/WalkOnGrass2.mp3", false);
		//sound->LoadSound("WalkOnWood1", "../Resource/Sound/SFX/WalkOnWood1.mp3", false);
		//sound->LoadSound("WalkOnWood2", "../Resource/Sound/SFX/WalkOnWood2.mp3", false);
		//sound->LoadSound("WalkOnCloth1", "../Resource/Sound/SFX/WalkOnCloth1.mp3", false);
		//sound->LoadSound("WalkOnCloth2", "../Resource/Sound/SFX/WalkOnCloth2.mp3", false);
		//sound->LoadSound("WalkOnSand1", "../Resource/Sound/SFX/WalkOnSand1.mp3", false);
		//sound->LoadSound("WalkOnSand2", "../Resource/Sound/SFX/WalkOnSand2.mp3", false);
		//sound->LoadSound("HitPlayer", "../Resource/Sound/SFX/HitPlayer.mp3", false);

		//Monster
		//sound->LoadSound("WalkZombie1", "../Resource/Sound/SFX/WalkZombie1.mp3", false);
		//sound->LoadSound("WalkZombie2", "../Resource/Sound/SFX/WalkZombie2.mp3", false);
		sound->LoadSound("AttackZombie", "../Resource/Sound/SFX/AttackZombie.mp3", false);
		sound->LoadSound("HitZombie", "../Resource/Sound/SFX/HitZombie.mp3", false);
		sound->LoadSound("DeathZombie", "../Resource/Sound/SFX/DeathZombie.mp3", false);

		//sound->LoadSound("WalkSkeleton1", "../Resource/Sound/SFX/WalkSkeleton1.mp3", false);
		//sound->LoadSound("WalkSkeleton2", "../Resource/Sound/SFX/WalkSkeleton2.mp3", false);
		sound->LoadSound("HitSkeleton", "../Resource/Sound/SFX/HitSkeleton.mp3", false);
		sound->LoadSound("DeathSkeleton", "../Resource/Sound/SFX/DeathSkeleton.mp3", false);

		//sound->LoadSound("WalkSlime1", "../Resource/Sound/SFX/WalkSlime1.mp3", false);
		//sound->LoadSound("WalkSlime2", "../Resource/Sound/SFX/WalkSlime2.mp3", false);
		sound->LoadSound("AttackSlime", "../Resource/Sound/SFX/AttackSlime.mp3", false);

		//sound->LoadSound("HitGolem", "../Resource/Sound/SFX/HitGolem.mp3", false);
		sound->LoadSound("DeathGolem", "../Resource/Sound/SFX/DeathGolem.mp3", false);
		sound->LoadSound("SwipeGolem", "../Resource/Sound/SFX/SwipeRedStone.mp3", false);
		sound->LoadSound("WalkGolem", "../Resource/Sound/SFX/WalkGolem.mp3", false);
		sound->LoadSound("FireProjectile", "../Resource/Sound/SFX/FireProjectile.mp3", false);

		sound->LoadSound("DeathCreeper", "../Resource/Sound/SFX/DeathCreeper.mp3", false);
		sound->LoadSound("ExplosionCreeper", "../Resource/Sound/SFX/ExplosionCreeper.mp3", false);

		sound->LoadSound("WalkEnder", "../Resource/Sound/SFX/WalkEnder.mp3", false);
		sound->LoadSound("HiddenEnder", "../Resource/Sound/SFX/HiddenEnder.mp3", false);
		sound->LoadSound("SproutEnder", "../Resource/Sound/SFX/SproutEnder.mp3", false);
		sound->LoadSound("CrossLaserEnder", "../Resource/Sound/SFX/CrossLaserEnder.mp3", false);
		sound->LoadSound("LineLaserEnder", "../Resource/Sound/SFX/LineLaserEnder.mp3", false);
		sound->LoadSound("ProjectileEnder", "../Resource/Sound/SFX/ProjectileEnder.mp3", false);
		sound->LoadSound("DeathEnder", "../Resource/Sound/SFX/DeathEnder.mp3", false);

		//Map
		//sound->LoadSound("OpenChest", "../Resource/Sound/SFX/OpenChest.mp3", false);
		//sound->LoadSound("CloseChest", "../Resource/Sound/SFX/CloseChest.mp3", false);

		//UI
		//sound->LoadSound("UsePotion", "../Resource/Sound/SFX/UsePotion.mp3", false);
		//sound->LoadSound("SuccessEnhance", "../Resource/Sound/SFX/SuccessEnhance.mp3", false);
		//sound->LoadSound("FailEnhance", "../Resource/Sound/SFX/FailEnhance.mp3", false);
		//sound->LoadSound("SuccessTrade", "../Resource/Sound/SFX/SuccessTrade.mp3", false);
		//sound->LoadSound("FailTrade", "../Resource/Sound/SFX/FailTrade.mp3", false);
		//sound->LoadSound("QuestClear", "../Resource/Sound/SFX/QuestClear.mp3", false);
		//sound->LoadSound("TalkNPC", "../Resource/Sound/SFX/TalkNPC.mp3", false);
	}

	/*----------------------Load Sprite----------------------------*/
	{
		resource->LoadSprite("Explode", 12);
		resource->LoadSprite("Walk", 7);
		resource->LoadSprite("Dead", 8);
	}

	/*--------------------Load Chunk----------------------*/
	{
		chunkload->LoadChunk(L"../../Reference/MapData/Stage1.chunkmap");
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
