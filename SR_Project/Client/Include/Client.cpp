#include "pch.h"
#include "MainApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    MainApp* Game = MainApp::Create(hInstance, nCmdShow);
    if (!Game)
        return FALSE;

    Game->Run();

    _ulong dwRef = Engine::Safe_Release(Game);
    if (dwRef)
    {
        MSG_BOX("MainApp Release Failed");
        return FALSE;
    }

    return 0;
}
