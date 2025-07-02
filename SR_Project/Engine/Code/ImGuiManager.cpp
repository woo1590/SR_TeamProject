#include "EnginePCH.h"

#ifdef USE_IMGUI

#include "ImGuiManager.h"
#include "GraphicDevice.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImGuiManager::ImGuiManager()
{
}

ImGuiManager::~ImGuiManager()
{
}

ImGuiManager* ImGuiManager::Create(HWND hWnd)
{
	ImGuiManager* Instance = new ImGuiManager;

	if (FAILED(Instance->Ready_ImGuiManager(hWnd)))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT ImGuiManager::Ready_ImGuiManager(HWND hWnd)
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(GraphicDevice::GetInstance()->GetDevice());

	return S_OK;
}

void ImGuiManager::BeginFrame()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto& [key, window] : Gui_WindowMap)
		window();
}

void ImGuiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::ShutDown()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::RegisterWindow(const std::wstring& key, GuiWindow window)
{
	Gui_WindowMap.emplace(key, std::move(window));
}

void ImGuiManager::UnRegisterWindow(const std::wstring& key)
{
	Gui_WindowMap.erase(key);
}

_bool ImGuiManager::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam) != 0;
}


void ImGuiManager::Free()
{
	ShutDown();
}

#endif //USE_IMGUI