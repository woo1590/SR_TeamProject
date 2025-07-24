#include "pch.h"
#include "MainApp.h"
#include "CubeMesh.h"
#include "QuadMesh.h"
#include "ArrowMesh.h"
#include "GraphicDevice.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "TimerManager.h"
#include "FrameManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "ImGuiManager.h"
#include "GameManager.h"
#include "RenderSystem.h"

//scene
#include "LoadingScene.h"
#include "TestScene.h"

MainApp::MainApp():Core(nullptr), GraphicDev(nullptr)
{

}

MainApp::~MainApp()
{

}

HRESULT MainApp::Ready_MainApp(HINSTANCE hInst, int nCmdShow)
{
    if (!InitWindow(hInst, nCmdShow))
        return E_FAIL;

    if (FAILED(Engine::GraphicDevice::GetInstance()->Ready_GraphicDev(hWnd, WINMODE::Mode_Win,
        windowSize.right,
        windowSize.bottom,
        &GraphicDev)))
        return E_FAIL;

    Core = Engine::EngineCore::GetInstance();
    Core->Ready_Engine(hWnd);

    if(FAILED(Core->GetTimerManager()->AddTimer(L"Timer_Immediate")))
        return E_FAIL;
    if (FAILED(Core->GetTimerManager()->AddTimer(L"Timer_FPS")))
        return E_FAIL;
    if (FAILED(Core->GetFrameManager()->AddFrame(L"Frame60", 60.f)))
        return E_FAIL;

    if(FAILED(GameManager::GetInstance()->Ready_GameManager()))
        return E_FAIL;
    if (FAILED(LoadDefaultResource()))
        return E_FAIL;

    EngineCore::GetInstance()->GetRenderSystem()->SetShader("DefaultPost");
    EngineCore::GetInstance()->GetSoundManager()->PlayBGM("IntroBGM");

    /*---------Start Scene----------*/
    Scene* Start = LoadingScene::Create(LOADID::Village);
    Core->GetSceneManager()->SetActiveScene(Start);

    isRunning = true;

	return S_OK;
}

void MainApp::Run()
{
    while (isRunning)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message) return;

            if (!TranslateAccelerator(msg.hwnd, hAccel, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        Core->GetTimerManager()->Set_DeltaTime(L"Timer_Immediate");

        _float Immediate_DT = Core->GetTimerManager()->Get_DeltaTime(L"Timer_Immediate");
        Immediate_DT = std::clamp(Immediate_DT, 0.f, 0.03f);

        Core->Tick(Immediate_DT);
    }
}

MainApp* MainApp::Create(HINSTANCE hInst, int nCmdShow)
{
    MainApp* Instance = new MainApp;

    if (FAILED(Instance->Ready_MainApp(hInst,nCmdShow)))
    {
        Engine::Safe_Release(Instance);
        MSG_BOX("MainApp Create Failed");
        Instance = nullptr;
    }

    return Instance;
}

LRESULT MainApp::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef USE_IMGUI
    if (EngineCore::GetInstance()->GetImGuiManager()->WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
#endif

    switch (msg)
    {
    case WM_INPUT:
    {
        static int accX = 0;
        static int accY = 0;

        UINT size = 0;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size,
            sizeof(RAWINPUTHEADER));

        std::vector<BYTE> buffer(size);
        RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.data());
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, raw, &size,
            sizeof(RAWINPUTHEADER));

        if (raw->header.dwType == RIM_TYPEMOUSE)
        {
            _float dx = (_float)raw->data.mouse.lLastX;
            _float dy = (_float)raw->data.mouse.lLastY;

            EngineCore::GetInstance()->GetInputSystem()->SetMouseDelta(_vec2(dx, dy));
        }
    }
    break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hWnd);
            break;
        default:
            break;
        }
        break;
    case WM_KEYUP:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

_bool MainApp::InitWindow(HINSTANCE hInst, int nCmdShow)
{
    hInstance = hInst;

    const wchar_t* CLASS_NAME = L"MyGameWindowClass";
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        OutputDebugString(L"[Engine] RegisterClassEx failed\n");
        return false;
    }

    windowSize = { 0, 0, (LONG)WINCX, (LONG)WINCY };
    //windowSize = originalWindowSize;

    AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);

    // ȭ�� �ػ� ���� 
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = windowSize.right - windowSize.left;
    int windowHeight = windowSize.bottom - windowSize.top;

    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    hWnd = CreateWindowW(
        CLASS_NAME, L"My Game Window",
        WS_OVERLAPPEDWINDOW,
        x, y,
        windowWidth,
        windowHeight,
        nullptr, nullptr, hInstance, nullptr
    );


    if (!hWnd) {
        OutputDebugString(L"[Engine] CreateWindow failed\n");
        return false;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    /*----------Cursor Lock-----------*/
    RECT clientRECT{};
    GetClientRect(hWnd, &clientRECT);

    POINT lt = { clientRECT.left,clientRECT.top };
    POINT rb = { clientRECT.right,clientRECT.bottom };

    ClientToScreen(hWnd, &lt);
    ClientToScreen(hWnd, &rb);
    RECT clipRect{
        lt.x,lt.y,rb.x,rb.y
    };

    ClipCursor(&clipRect);

    ShowCursor(FALSE);

   /*--------------Raw Input---------------*/
   RAWINPUTDEVICE rid{};
   rid.usUsagePage = 0x01;
   rid.usUsage = 0x02;
   rid.dwFlags = RIDEV_INPUTSINK;
   rid.hwndTarget = hWnd;

   RegisterRawInputDevices(&rid, 1, sizeof(rid));

    return true;
}

void MainApp::Free()
{
    Core->DestroyInstance();
    GraphicDev->DestroyInstance();
}

HRESULT MainApp::LoadDefaultResource()
{
    auto resource = EngineCore::GetInstance()->GetResourceManager();
    auto sound = EngineCore::GetInstance()->GetSoundManager();

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
        resource->LoadShader("../Resource/Shader/DefaultPost.fx", "DefaultPost");
        resource->LoadShader("../Resource/Shader/FireworkShader.fx", "FireworkShader");
        resource->LoadShader("../Resource/Shader/BloomShader.fx", "BloomShader");
        resource->LoadShader("../Resource/Shader/AlphaShader.fx", "AlphaShader");
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
        sound->LoadSound("IntroBGM", "../Resource/Sound/BGM/Intro_BGM.mp3", true);
        sound->LoadSound("VillageBGM", "../Resource/Sound/BGM/Village_BGM.mp3", true);
        sound->LoadSound("Stage1BGM", "../Resource/Sound/BGM/Stage1_BGM.mp3", true);
        sound->LoadSound("Boss_EnderBGM", "../Resource/Sound/BGM/Boss_EnderBGM.mp3", true);
        sound->LoadSound("Stage2BGM", "../Resource/Sound/BGM/Stage2BGM.mp3", true);
        sound->LoadSound("Boss_GolemBGM", "../Resource/Sound/BGM/Boss_GolemBGM.mp3", true);

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
        //sound->LoadSound("HitPlayer", "../Resource/Sound/SFX/HitPlayer.mp3", false);
        sound->LoadSound("WalkOnDefault1", "../Resource/Sound/SFX/WalkOnDefault1.mp3", false);
        sound->LoadSound("WalkOnDefault2", "../Resource/Sound/SFX/WalkOnDefault2.mp3", false);
        sound->LoadSound("WalkOnGrass1", "../Resource/Sound/SFX/WalkOnGrass1.mp3", false);
        sound->LoadSound("WalkOnGrass2", "../Resource/Sound/SFX/WalkOnGrass2.mp3", false);
        sound->LoadSound("WalkOnWood1", "../Resource/Sound/SFX/WalkOnWood1.mp3", false);
        sound->LoadSound("WalkOnWood2", "../Resource/Sound/SFX/WalkOnWood2.mp3", false);
        sound->LoadSound("WalkOnStone1", "../Resource/Sound/SFX/WalkOnStone1.mp3", false);
        sound->LoadSound("WalkOnStone2", "../Resource/Sound/SFX/WalkOnStone2.mp3", false);
        sound->LoadSound("WalkOnCloth1", "../Resource/Sound/SFX/WalkOnCloth1.mp3", false);
        sound->LoadSound("WalkOnCloth2", "../Resource/Sound/SFX/WalkOnCloth2.mp3", false);
        sound->LoadSound("WalkOnSand1", "../Resource/Sound/SFX/WalkOnSand1.mp3", false);
        sound->LoadSound("WalkOnSand2", "../Resource/Sound/SFX/WalkOnSand2.mp3", false);
        sound->LoadSound("Statikk", "../Resource/Sound/SFX/Statikk.mp3", false);
        sound->LoadSound("Charge", "../Resource/Sound/SFX/Charge.mp3", false);

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
        sound->LoadSound("StartExplosionCreeper", "../Resource/Sound/SFX/StartExplosionCreeper.mp3", false);

        sound->LoadSound("WalkEnder", "../Resource/Sound/SFX/WalkEnder.mp3", false);
        sound->LoadSound("HiddenEnder", "../Resource/Sound/SFX/HiddenEnder.mp3", false);
        sound->LoadSound("SproutEnder", "../Resource/Sound/SFX/SproutEnder.mp3", false);
        sound->LoadSound("CrossLaserEnder", "../Resource/Sound/SFX/CrossLaserEnder.mp3", false);
        sound->LoadSound("LineLaserEnder", "../Resource/Sound/SFX/LineLaserEnder.mp3", false);
        sound->LoadSound("ProjectileEnder", "../Resource/Sound/SFX/ProjectileEnder.mp3", false);
        sound->LoadSound("DeathEnder", "../Resource/Sound/SFX/DeathEnder.mp3", false);

        sound->LoadSound("DeathBlastling", "../Resource/Sound/SFX/DeathBlastling.mp3", false);
        sound->LoadSound("IdleBlastling", "../Resource/Sound/SFX/IdleBlastling.mp3", false);
        sound->LoadSound("AttackBlastling", "../Resource/Sound/SFX/AttackBlastling.mp3", false);
        sound->LoadSound("HurtBlastling", "../Resource/Sound/SFX/HurtBlastling.mp3", false);
        
        sound->LoadSound("AttackShulker", "../Resource/Sound/SFX/AttackShulker.mp3", false);
        sound->LoadSound("DeathShulker", "../Resource/Sound/SFX/DeathShulker.mp3", false);

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
        sound->LoadSound("Typing", "../Resource/Sound/SFX/Typing.mp3", true);
        sound->LoadSound("switch13", "../Resource/Sound/SFX/switch13.ogg", false);
        sound->LoadSound("PullLever", "../Resource/Sound/SFX/PullLever.mp3", false);
        sound->LoadSound("OperateBridge", "../Resource/Sound/SFX/OperateBridge.mp3", false);
        sound->LoadSound("LevelUp", "../Resource/Sound/SFX/LevelUp.mp3", false);
    }

    /*----------------------Load Sprite----------------------------*/
    {
        resource->LoadSprite("Explode", 12);
        resource->LoadSprite("Walk", 7);
        resource->LoadSprite("Dead", 8);
    }

    /*----------------------Load UI Resource-----------------------*/
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
        {L"loadingscene_stage0",      eUI, -1},
        {L"loadingscene_stage1",      eUI, -1},
        {L"loadingscene_stage2",      eUI, -1},
        {L"loadingscene_stage3",      eUI, -1},
        {L"loadingscene_tip1",        eUI, -1},
        {L"loadingscene_tip2",        eUI, -1},
        {L"loadingscene_tip3",        eUI, -1},
        {L"loadingscene_tip4",        eUI, -1},
        {L"loadingscene_tip5",        eUI, -1},
        {L"loadingscene_tip6",        eUI, -1},
        {L"loadingscene_tip7",        eUI, -1},
        {L"loadingscene_tip8",        eUI, -1},
        {L"loadingscene_tip9",        eUI, -1},
        {L"loadingscene_tip10",       eUI, -1},
        {L"loadingscene_tip11",       eUI, -1},
        {L"shop_back",                eUI, -1},

        // =====================================================================
        // Layer 0 (UI 베이스)
        // =====================================================================
        {L"hotbar_back",              eUI, 0},
        {L"gearstrength_back",        eUI, 0},
        {L"level_back",               eUI, 0},
        {L"deathframe",               eUI, 0},

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
        {L"levelup_banner",           eUI, 1},
        {L"levelup",                  eUI, 1},
        {L"shop_emerald",             eUI, 1},
        {L"shop_button",              eUI, 1},


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
        {L"cooldown_rect",            eUI, 2},


        // =====================================================================
        // Layer 3
        // =====================================================================
        {L"mouse_left",               eUI, 3},
        {L"mouse_right",              eUI, 3},
        {L"dash_icon",                eUI, 3},
        {L"quickslot_plus",           eUI, 3},
        {L"questpanel",               eUI, 3},
        {L"enemy_hpbarback",          eUI, 3},
        {L"arrows",                   eUI, 3},
        {L"arrow",                    eUI, 3},
        {L"shopslot",                 eUI, 3},
        {L"enderworld",               eUI, 3},

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
        {L"cooldown_front",           eUI, 6},
        {L"atri",                     eUI, 9},
        {L"emerald_sword",            eUI, 10},
        {L"panel",                    eUI, 10},
        {L"angry",                    eUI, 10},

        {L"0",                        eUI, 9},
        {L"1",                        eUI, 9},
        {L"2",                        eUI, 9},
        {L"3",                        eUI, 9},
        {L"4",                        eUI, 9},
        {L"5",                        eUI, 9},
        {L"6",                        eUI, 9},
        {L"7",                        eUI, 9},
        {L"8",                        eUI, 9}, 
        {L"9",                        eUI, 9},
        {L"10",                       eUI, 9}, 
        {L"11",                       eUI, 9},
        {L"12",                       eUI, 9},
        {L"13",                       eUI, 9},
        {L"14",                       eUI, 9}, 
        {L"15",                       eUI, 9},
        {L"16",                       eUI, 9}, 
        {L"17",                       eUI, 9},
        {L"18",                       eUI, 9}, 
        {L"19",                       eUI, 9},
        {L"20",                       eUI, 9},
        {L"21",                       eUI, 9},
        {L"22",                       eUI, 9},
        {L"23",                       eUI, 9},
        

        {L"gray",                     eUI, 9},
        {L"gray2",                    eUI, 9},

        {L"stage0",                   eUI, 9},
        {L"stage1",                   eUI, 9},
        {L"stage2",                   eUI, 9},
        {L"button_left",              eUI, 9},
        {L"button_right",             eUI, 9},

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
    }

    return S_OK;
}
