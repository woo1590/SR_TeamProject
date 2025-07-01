#include "pch.h"
#include "MainApp.h"

#include "GraphicDevice.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "TimerManager.h"
#include "FrameManager.h"
#include "SceneManager.h"
#include "ImGuiManager.h"

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

   

    /*---------Start Scene----------*/
    Scene* Start = LoadingScene::Create(LOADID::TestScene);
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
        if (true/*Core->GetFrameManager()->IsPermitCall(L"Frame60", Immediate_DT)*/)
        {
            //Core->GetTimerManager()->Set_DeltaTime(L"Timer_FPS");

            //_float FPS_DT = Core->GetTimerManager()->Get_DeltaTime(L"Timer_FPS");

            Core->Tick(Immediate_DT);
        }
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
    if (EngineCore::GetInstance()->GetImGuiManager()->WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

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

    // 화면 해상도 정보 
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

    //ClipCursor(&clipRect);

    //ShowCursor(FALSE);

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
