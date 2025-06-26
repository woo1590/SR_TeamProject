#pragma once

namespace Engine
{
    class GraphicDevice;
    class EngineCore;
}

class Test;
class Test_Teapot;
class MainApp :
    public Base
{
private:
    explicit MainApp();
    virtual ~MainApp();

public:
    static MainApp* Create(HINSTANCE hInst, int nCmdShow);
    void Run();

private:
    /*----------------------------------*/
    HRESULT Ready_MainApp(HINSTANCE hInst, int nCmdShow);

    _bool InitWindow(HINSTANCE hInst, int nCmdShow);
    HWND GetWindowHandle() const { return hWnd; }
    RECT GetWindowSize() const { return originalWindowSize; }
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void Free()override;
private:
    HWND hWnd;
    HINSTANCE hInstance;
    RECT windowSize;
    RECT originalWindowSize;
    MSG msg;
    HACCEL hAccel;

    /*--------------------*/
    _bool isRunning;
    Engine::GraphicDevice* GraphicDev;
    Engine::EngineCore* Core;

    /*---------test obj-----*/
    Test* test;
    Test_Teapot* teapot;
};

