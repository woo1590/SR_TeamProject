#include "EnginePCH.h"
#include "ImGuiManager.h"

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
	return S_OK;
}

void ImGuiManager::BeginFrame()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

void ImGuiManager::EndFrame()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::ShutDown()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


void ImGuiManager::Free()
{
	ShutDown();
}
