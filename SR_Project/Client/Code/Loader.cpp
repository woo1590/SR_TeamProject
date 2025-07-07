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
	auto cube = CubeMesh::Create();
	resource->LoadMesh(L"Cube_Mesh", cube);
	resource->LoadResource(L"../Resource/Texture/SkyBox/burger3.dds", L"Basic_SkyBox", TEXTURE::Tex_Cube, L"SkyBox_Mtrl");
	
	//////////////////////////////////////////////////////////////////////////////////
	resource->LoadResource(L"../Resource/Asset/HY/Body2.dds", L"Body", TEXTURE::Tex_Cube, L"ZombieBody_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/ZombieHead.dds", L"Head", TEXTURE::Tex_Cube, L"ZombieHead_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/Arm.dds", L"Arm", TEXTURE::Tex_Cube, L"ZombieArm_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/Leg.dds", L"Leg", TEXTURE::Tex_Cube, L"ZombieLeg_Mtrl");
	
	//////////////////////////////////////////////////////////////////////////////////
	resource->LoadResource(L"../Resource/Asset/JW/playerHead.dds", L"playerHead", TEXTURE::Tex_Cube, L"playerHead_Mtrl");
	resource->LoadResource(L"../Resource/Asset/JW/playerBody.dds", L"playerBody", TEXTURE::Tex_Cube, L"playerBody_Mtrl");
	resource->LoadResource(L"../Resource/Asset/JW/playerRightArm.dds", L"playerRightArm", TEXTURE::Tex_Cube, L"playerRightArm_Mtrl");
	resource->LoadResource(L"../Resource/Asset/JW/playerLeftArm.dds", L"playerLeftArm", TEXTURE::Tex_Cube, L"playerLeftArm_Mtrl");
	resource->LoadResource(L"../Resource/Asset/JW/playerRightLeg.dds", L"playerRightLeg", TEXTURE::Tex_Cube, L"playerRightLeg_Mtrl");
	resource->LoadResource(L"../Resource/Asset/JW/playerLeftLeg.dds", L"playerLeftLef", TEXTURE::Tex_Cube, L"playerLeftLeg_Mtrl");
	resource->LoadResource(L"../Resource/Asset/JW/sword.dds", L"sword", TEXTURE::Tex_Cube, L"sword_Mtrl");
	resource->LoadResource(L"../Resource/Asset/JW/bow.dds", L"bow", TEXTURE::Tex_Cube, L"bow_Mtrl");
	resource->LoadResource(L"../Resource/Asset/JW/arrow.dds", L"arrow", TEXTURE::Tex_Cube, L"arrow_Mtrl");

	/////////////////////////////////////////////////////////////////////////////////
	resource->LoadResource(L"../Resource/Asset/HY/Creeper_Face.dds", L"CreeperFace", TEXTURE::Tex_Cube, L"CreeperFace_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/Creeper_Body.dds", L"CreeperBody", TEXTURE::Tex_Cube, L"CreeperBody_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/Creeper_Leg.dds", L"CreeperLeg", TEXTURE::Tex_Cube, L"CreeperLeg_Mtrl");

	/////////////////////////////////////////////////////////////////////////////////
	resource->LoadResource(L"../Resource/Asset/HY/Skeleton_Body.dds", L"SkeletonBody", TEXTURE::Tex_Cube, L"SkeletonBody_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/Skeleton_Face.dds", L"SkeletonFace", TEXTURE::Tex_Cube, L"SkeletonFace_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/Skeleton_Bone.dds", L"SkeletonBone", TEXTURE::Tex_Cube, L"SkeletonBone_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/bow.dds", L"bow", TEXTURE::Tex_Cube, L"Bow_Mtrl");

	/////////////////////////////////////////////////////////////////////////////////
	resource->LoadResource(L"../Resource/Asset/HY/redgolem_body.dds", L"RedGolem_Body", TEXTURE::Tex_Cube, L"RedGolemBody_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/redgolem_face.dds", L"RedGolem_Face", TEXTURE::Tex_Cube, L"RedGolemFace_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/redgolem_finger.dds", L"RedGolem_Finger", TEXTURE::Tex_Cube, L"RedGolemFinger_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/redgolem_foot.dds", L"RedGolem_Foot", TEXTURE::Tex_Cube, L"RedGolemFoot_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/redgolem_arm.dds", L"RedGolem_Arm", TEXTURE::Tex_Cube, L"RedGolemArm_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/redgolem_hand.dds", L"RedGolem_Hand", TEXTURE::Tex_Cube, L"RedGolemHand_Mtrl");
	resource->LoadResource(L"../Resource/Asset/HY/redgolem_lightblock.dds", L"Redgolem_Lightblock", TEXTURE::Tex_Cube, L"RedgolemLightblock_Mtrl");

	/////////////////////////////////////////////////////////////////////////////////
	resource->LoadResource(L"../Resource/Texture/Block/DirtBlock.dds", L"DirtBlock", TEXTURE::Tex_Cube, L"DirtBlock_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Block/GrassBlock.dds", L"GrassBlock", TEXTURE::Tex_Cube, L"GrassBlock_Mtrl");

	resource->LoadResource(L"../Resource/Texture/Block/WoodBlock.dds", L"WoodBlock", TEXTURE::Tex_Cube, L"WoodBlock_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Block/WoodPlank.dds", L"WoodPlank", TEXTURE::Tex_Cube, L"WoodPlank_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Block/WoodPlank_Half.dds", L"WoodPlank_Half", TEXTURE::Tex_Cube, L"WoodPlank_Half_Mtrl");

	resource->LoadResource(L"../Resource/Texture/Block/Stone.dds", L"Stone", TEXTURE::Tex_Cube, L"Stone_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Block/Stone_Half.dds", L"Stone_Half", TEXTURE::Tex_Cube, L"Stone_Half_Mtrl");

	resource->LoadResource(L"../Resource/Texture/Block/CobbleStone.dds", L"CobbleStone", TEXTURE::Tex_Cube, L"CobbleStone_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Block/CobbleStone_Half.dds", L"CobbleStone_Half", TEXTURE::Tex_Cube, L"CobbleStone_Half_Mtrl");

	resource->LoadResource(L"../Resource/Texture/Block/SmoothStone.dds", L"SmoothStone", TEXTURE::Tex_Cube, L"SmoothStone_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Block/SmoothStone_Half.dds", L"SmoothStone_Half", TEXTURE::Tex_Cube, L"SmoothStone_Half_Mtrl");

	resource->LoadResource(L"../Resource/Texture/Block/StoneBrick.dds", L"StoneBrick", TEXTURE::Tex_Cube, L"StoneBrick_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Block/MossyStoneBrick.dds", L"MossyStoneBrick", TEXTURE::Tex_Cube, L"MossyStoneBrick_Mtrl");

	resource->LoadResource(L"../Resource/Texture/Block/Glass.dds", L"Glass", TEXTURE::Tex_Cube, L"Glass_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Block/Leaf.dds", L"Leaf", TEXTURE::Tex_Cube, L"Leaf_Mtrl");

	resource->LoadResource(L"../Resource/Texture/Block/Lever.dds", L"Lever", TEXTURE::Tex_Cube, L"Lever_Mtrl");

	resource->LoadResource(L"../Resource/Texture/Block/ChestDown.dds", L"ChestDown", TEXTURE::Tex_Cube, L"ChestDown_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Block/ChestUp.dds", L"ChestUp", TEXTURE::Tex_Cube, L"ChestUp_Mtrl");
	resource->LoadResource(L"../Resource/Texture/Block/ChestLock.dds", L"ChestLock", TEXTURE::Tex_Cube, L"ChestLock_Mtrl");

	resource->LoadResource(L"../Resource/Texture/Block/IronCage.dds", L"IronCage", TEXTURE::Tex_Cube, L"IronCage_Mtrl");

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
		{L"quickslot_hover",    eUI},
		{L"inventorybtn",       eUI},
		{L"mapbtn",             eUI},
		{L"mouse_left",         eUI},
		{L"mouse_right",        eUI},
		{L"dash_icon",          eUI},
		{L"quickslot_plus",     eUI},
		{L"exitbtn",            eUI},
		{L"exitbtn_hover",      eUI},
		{L"emerald_sword",      eUI},
		{L"gearslot_plus",      eUI},
		{L"gearslot",           eUI},
		{L"gearslot_hover",     eUI},
		{L"gearslot_highlight", eUI},
		{L"hp_potion",          eUI},
		{L"gearstrength",       eUI},
		{L"gearstrength_back",  eUI},
		{L"level_back",         eUI},
		{L"level_front",        eUI},
		{L"filter",             eUI},
		{L"filter_hover",       eUI},
		{L"swordfilter",        eUI},
		{L"swordfilter_hover",  eUI},
		{L"arrowfilter",        eUI},
		{L"arrowfilter_hover",  eUI},
		{L"armorfilter",        eUI},
		{L"armorfilter_hover",  eUI},
		{L"potionfilter",       eUI},
		{L"potionfilter_hover", eUI},
		{L"enchantfilter",      eUI},
		{L"enchantfilter_hover",eUI},
		{L"costumefilter",      eUI},
		{L"costumefilter_hover",eUI},
		{L"inventoryslot",      eUI},
		{L"itemslot",           eUI},
		{L"slot_selected",      eUI},
		{L"inventory_emerald",  eUI},
		{L"inventory_enchant",  eUI},
		{L"icon_enchant",       eUI},
		{L"scroll_back",        eUI},
		{L"inventory_sword",    eUI},
		{L"inventory_bow",      eUI},
		{L"inventory_wolfarmor",eUI},
		{L"inventory_rocket",   eUI},
		{L"inventory_fishing",  eUI},
		{L"questpanel",         eUI},
		{L"quest_icon",         eUI},
		{L"quest_icon2",        eUI},

	};

	static const unordered_map<wstring, int> uiLayers = {
		{L"Logo",               -1},
		{L"loadingscene",       -1},

		{L"hotbar_back",         0},
		{L"hpbar_back",          1},
		{L"expbar_back",         1},
		{L"expbar_back",         1},
		{L"icon_emerald",        1},
		{L"icon_enchant",        1},
		{L"expbar_front",        2},
		{L"quickslot",           2},
		{L"hpbar_front",         2},
		{L"quickslot_hover", 2},
		{L"arrow_slot",          2},
		{L"mouse_left",          3},
		{L"mouse_right",         3},
		{L"dash_icon",           3},
		{L"quickslot_plus",      3},
		{L"Shield",              5},
		{L"inventorybtn",        5},
		{L"mapbtn",              5},
		{L"hp_potion",           5},
		
		{L"gearslot_highlight", 1},
		{L"gearslot",           1},
		{L"gearslot_plus",      2},
		{L"gearslot_hover",     2},
		
		{L"emerald_sword",      10},

		{L"InventoryPanel",     -1},
		{L"exitbtn",           101},
		{L"exitbtn_hover",     101},

		{L"gearstrength_back",   0},
		{L"gearstrength",        1},
		{L"level_front",         1},
		{L"level_back",          0},
		{L"filter",              1},
		{L"filter_hover",        1},
		{L"swordfilter",         2},
		{L"swordfilter_hover",   2},
		{L"arrowfilter",         2},
		{L"arrowfilter_hover",   2},
		{L"armorfilter",         2},
		{L"armorfilter_hover",   2},
		{L"potionfilter",        2},
		{L"potionfilter_hover",  2},
		{L"enchantfilter",       2},
		{L"enchantfilter_hover", 2},
		{L"costumefilter",       2},
		{L"costumefilter_hover", 2},
		{L"inventoryslot",       1},
		{L"itemslot",            1},
		{L"slot_selected",       2},
		{L"inventory_emerald",   1},
		{L"inventory_enchant",   1},
		{L"scroll_back",         1},
		{L"inventory_sword",     5},
		{L"inventory_bow",       5},
		{L"inventory_wolfarmor", 5},
		{L"inventory_rocket",    5},
		{L"inventory_fishing",   5},
		{L"questpanel",          3},
		{L"quest_icon",          4},
		{L"quest_icon2",         4},

		{L"Cursor",           1000},

	};

	auto rm = EngineCore::GetInstance()->GetResourceManager();
	const wstring basePath = L"../Resource/Asset/Jehyun/";
	
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
