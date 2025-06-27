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

    using GuiWindow = std::function<void()>;
public:
    static ImGuiManager* Create(HWND hWnd);
    HRESULT Ready_ImGuiManager(HWND hWnd);
    void BeginFrame();
    void Render();
    void ShutDown();

    void RegisterWindow(const std::wstring& key, GuiWindow window);
    void UnRegisterWindow(const std::wstring& key);
private:
    void Free()override;

    std::unordered_map <std::wstring, GuiWindow> Gui_WindowMap;
};

END
