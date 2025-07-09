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
		//hr = load_UIResources();
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

	auto cube = CubeMesh::Create();
	resource->LoadMesh("Cube_Mesh", cube);
	/*--------------------Load Shader---------------------------------*/
	resource->LoadShader("../Resource/Shader/SkyBox.fx", "SkyBoxShader");
	resource->LoadShader("../Resource/Shader/BasicShader.fx", "BasicShader");
	resource->LoadShader("../Resource/Shader/ChunkShader.fx", "ChunkShader");

	resource->LoadMaterial("../Resource/Material/SkyBox.json");
	resource->LoadMaterial("../Resource/Material/Monsters.json");
	resource->LoadMaterial("../Resource/Material/Player.json");
	resource->LoadMaterial("../Resource/Material/Blocks.json");
	resource->LoadMaterial("../Resource/Material/Items.json");
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
		{L"map_node",           eUI},
		{L"worldmap",           eUI},
		{L"locked_node",        eUI},
		{L"locked_node_back",   eUI},
		{L"map_node_front",     eUI},
		{L"map_node_hover",     eUI},
		{L"locked_node_hover",  eUI},
		{L"loading_stone",      eUI},
		{L"loadingscene_lobby", eUI},
		{L"worldmap_textpanel", eUI},
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
		{L"map_node",            4},
		{L"worldmap",           -1},
		{L"locked_node",         2},
		{L"locked_node_back",    1},
		{L"map_node_front",      2},
		{L"Cursor",           1000},
		{L"map_node_hover",      5},
		{L"locked_node_hover",   5},
		{L"loadingscene_lobby",  0},
		{L"loading_stone",       1},
		{L"worldmap_textpanel",  5},
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
