#include "pch.h"
#include "Loader.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "StaticMesh.h"
#include "TerrainMesh.h"
#include "CubeMesh.h"
#include "Material.h"
#include "ArrowMesh.h"

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

	if (FAILED(load_UIResources()))
		return E_FAIL;

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
	case LOADID::Village:
		hr = Load_Village();
		break;
	case LOADID::Stage1:
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

HRESULT Loader::Load_TestScene()
{
	auto resource = EngineCore::GetInstance()->GetResourceManager();
	auto sound = EngineCore::GetInstance()->GetSoundManager();
	/*----------------------Load Mesh----------------------------*/
	{
		auto cube = CubeMesh::Create();
		resource->LoadMesh("Cube_Mesh", cube);
		auto arrow = ArrowMesh::Create();
		resource->LoadMesh("Arrow_Mesh", arrow);
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
	}

	/*---------------------Load Material--------------------------*/
	{
		resource->LoadMaterial("../Resource/Material/SkyBox.json");
		resource->LoadMaterial("../Resource/Material/Monsters.json");
		resource->LoadMaterial("../Resource/Material/Player.json");
		resource->LoadMaterial("../Resource/Material/Blocks.json");
		resource->LoadMaterial("../Resource/Material/Items.json");
		resource->LoadMaterial("../Resource/Material/Particle.json");
	}

	/*----------------------Load Sound-----------------------------*/
	{
		sound->LoadSound("TestBGM", "../Resource/Sound/BGM/Minecraft.mp3", true);
	}

	/*----------------------Load Sprite----------------------------*/
	{
		resource->LoadSprite("Explosion", 90);
		resource->LoadSprite("Walk", 7);
	}
	return S_OK;
}

HRESULT Loader::Load_Village()
{
	auto resource = EngineCore::GetInstance()->GetResourceManager();
	auto sound = EngineCore::GetInstance()->GetSoundManager();
	/*----------------------Load Mesh----------------------------*/
	{
		auto cube = CubeMesh::Create();
		resource->LoadMesh("Cube_Mesh", cube);
		auto arrow = ArrowMesh::Create();
		resource->LoadMesh("Arrow_Mesh", arrow);
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
	}

	/*---------------------Load Material--------------------------*/
	{
		resource->LoadMaterial("../Resource/Material/SkyBox.json");
		resource->LoadMaterial("../Resource/Material/Monsters.json");
		resource->LoadMaterial("../Resource/Material/Player.json");
		resource->LoadMaterial("../Resource/Material/Blocks.json");
		resource->LoadMaterial("../Resource/Material/Items.json");
		resource->LoadMaterial("../Resource/Material/Particle.json");
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
		//sound->LoadSound("HitZombie", "../Resource/Sound/SFX/HitZombie.mp3", false);
		sound->LoadSound("DeathZombie", "../Resource/Sound/SFX/DeathZombie.mp3", false);

		//sound->LoadSound("WalkSkeleton1", "../Resource/Sound/SFX/WalkSkeleton1.mp3", false);
		//sound->LoadSound("WalkSkeleton2", "../Resource/Sound/SFX/WalkSkeleton2.mp3", false);
		sound->LoadSound("DeathSkeleton", "../Resource/Sound/SFX/DeathSkeleton.mp3", false);

		//sound->LoadSound("WalkSlime1", "../Resource/Sound/SFX/WalkSlime1.mp3", false);
		//sound->LoadSound("WalkSlime2", "../Resource/Sound/SFX/WalkSlime2.mp3", false);

		//sound->LoadSound("HitGolem", "../Resource/Sound/SFX/HitGolem.mp3", false);
		sound->LoadSound("DeathGolem", "../Resource/Sound/SFX/DeathGolem.mp3", false);

		sound->LoadSound("DeathCreeper", "../Resource/Sound/SFX/DeathCreeper.mp3", false);

		//sound->LoadSound("DeathEnder", "../Resource/Sound/SFX/DeathEnder.mp3", false);

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
		resource->LoadSprite("Explosion", 90);
		resource->LoadSprite("Walk", 7);
		resource->LoadSprite("Dead", 8);
	}
	return S_OK;
}

HRESULT Loader::load_UIResources()
{
	// UI 리소스 정보를 담는 통합 구조체
	struct UIResourceInfo
	{
		const wchar_t* name;
		TEXTURE type;
		std::optional<int> layer; // 레이어가 없는 리소스는 std::nullopt
	};

	const auto eUI = TEXTURE::Tex_Diffuse;

	// 모든 UI 리소스를 하나의 정적 배열에서 관리 (레이어 순으로 정렬)
	static const UIResourceInfo uiResources[] = {
		// =====================================================================
		// Layer -1 (가장 뒤에 있는 배경 이미지)
		// =====================================================================
		{L"Logo",                     eUI, -1},
		{L"loadingscene",             eUI, -1},
		{L"InventoryPanel",           eUI, -1},
		{L"worldmap",                 eUI, -1},
		{L"player_deathui",           eUI, -1},

		// =====================================================================
		// Layer 0 (UI 베이스)
		// =====================================================================
		{L"hotbar_back",              eUI, 0},
		{L"gearstrength_back",        eUI, 0},
		{L"level_back",               eUI, 0},
		{L"deathframe",               eUI, 0},
		{L"loadingscene_lobby",       eUI, 0},

		// =====================================================================
		// Layer 1
		// =====================================================================
		{L"expbar_back",              eUI, 1},
		{L"hpbar_white",              eUI, 1},
		{L"icon_emerald",             eUI, 1},
		{L"icon_enchant",             eUI, 1},
		{L"gearslot",                 eUI, 1},
		{L"gearslot_highlight",       eUI, 1},
		{L"gearstrength",             eUI, 1},
		{L"level_front",              eUI, 1},
		{L"filter",                   eUI, 1},
		{L"filter_hover",             eUI, 1},
		{L"inventoryslot",            eUI, 1},
		{L"itemslot",                 eUI, 1},
		{L"inventory_emerald",        eUI, 1},
		{L"inventory_enchant",        eUI, 1},
		{L"scroll_back",              eUI, 1},
		{L"locked_node_back",         eUI, 1},
		{L"loading_stone",            eUI, 1},
		{L"boss_icon",                eUI, 1},

		// =====================================================================
		// Layer 2
		// =====================================================================
		{L"expbar_front",             eUI, 2},
		{L"quickslot",                eUI, 2},
		{L"quickslot_hover",          eUI, 2},
		{L"arrow_slot",               eUI, 2},
		{L"gearslot_plus",            eUI, 2},
		{L"gearslot_hover",           eUI, 2},
		{L"slot_selected",            eUI, 2},
		{L"swordfilter",              eUI, 2},
		{L"swordfilter_hover",        eUI, 2},
		{L"arrowfilter",              eUI, 2},
		{L"arrowfilter_hover",        eUI, 2},
		{L"armorfilter",              eUI, 2},
		{L"armorfilter_hover",        eUI, 2},
		{L"potionfilter",             eUI, 2},
		{L"potionfilter_hover",       eUI, 2},
		{L"enchantfilter",            eUI, 2},
		{L"enchantfilter_hover",      eUI, 2},
		{L"costumefilter",            eUI, 2},
		{L"costumefilter_hover",      eUI, 2},
		{L"locked_node",              eUI, 2},
		{L"map_node_front",           eUI, 2},

		// =====================================================================
		// Layer 3
		// =====================================================================
		{L"mouse_left",               eUI, 3},
		{L"mouse_right",              eUI, 3},
		{L"dash_icon",                eUI, 3},
		{L"quickslot_plus",           eUI, 3},
		{L"questpanel",               eUI, 3},
		{L"enemy_hpbarback",          eUI, 3},

		// =====================================================================
		// Layer 4
		// =====================================================================
		{L"hpbar_back",               eUI, 4},
		{L"quest_icon",               eUI, 4},
		{L"map_node",                 eUI, 4},
		{L"enemy_hpbarfront",         eUI, 4},
		{L"boss_hpbarfront",          eUI, 4},

		// =====================================================================
		// Layer 5 (주요 아이콘 및 버튼)
		// =====================================================================
		{L"Shield",                   eUI, 5},
		{L"hpbar_front",              eUI, 5},
		{L"inventorybtn",             eUI, 5},
		{L"mapbtn",                   eUI, 5},
		{L"hp_potion",                eUI, 5},
		{L"debugui",                  eUI, 5},
		{L"inventory_sword",          eUI, 5},
		{L"inventory_bow",            eUI, 5},
		{L"inventory_wolfarmor",      eUI, 5},
		{L"inventory_spear",          eUI, 5},
		{L"inventory_crossbow",       eUI, 5},
		{L"inventory_boost",          eUI, 5},
		{L"inventory_ghostcloak",     eUI, 5},
		{L"inventory_rocket",         eUI, 5},
		{L"inventory_fishing",        eUI, 5},
		{L"map_node_hover",           eUI, 5},
		{L"locked_node_hover",        eUI, 5},
		{L"worldmap_textpanel",       eUI, 5},
		{L"pig",                      eUI, 5},

		// =====================================================================
		// Layer 6+ (상위 레이어)
		// =====================================================================
		{L"particle",                 eUI, 6},
		{L"atri",                     eUI, 9},
		{L"emerald_sword",            eUI, 10},
		{L"panel",                    eUI, 10},
		{L"angry",                    eUI, 10},

		// =====================================================================
		// Layer 100+ (팝업 및 최상단 UI)
		// =====================================================================
		{L"exitbtn",                  eUI, 101},
		{L"exitbtn_hover",            eUI, 101},
		{L"Cursor",                   eUI, 1000},
	};

	auto rm = EngineCore::GetInstance()->GetResourceManager();
	const wstring basePath = L"../Resource/Asset/Jehyun/";

	for (const auto& resource : uiResources)
	{
		rm->LoadTexture(basePath + resource.name + L".png", resource.name, resource.type);

		if (resource.layer.has_value())
			rm->RegisterUILayer(resource.name, resource.layer.value());
	}

	return S_OK;
}

void Loader::Free()
{
	WaitForSingleObject(H_Thread, INFINITE);

	CloseHandle(H_Thread);

	DeleteCriticalSection(&Crt);
}
