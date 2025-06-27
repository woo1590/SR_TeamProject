#pragma once

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

BEGIN(Engine)

class ENGINE_DLL ImGuiManager :
    public Base
{
private:
    ImGuiManager();
    virtual ~ImGuiManager();

public:
    static ImGuiManager* Create(HWND hWnd);
    HRESULT Ready_ImGuiManager(HWND hWnd);
    void BeginFrame();
    void EndFrame();
    void ShutDown();

private:
    void Free()override;

};

END
