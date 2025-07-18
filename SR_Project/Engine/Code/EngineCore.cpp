#include "EnginePCH.h"
#include "EngineCore.h"
#include "GraphicDevice.h"
#include "FrameManager.h"
#include "TimerManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "LightSystem.h"
#include "InputSystem.h"
#include "SoundManager.h"
#include "ChunkLoader.h"
#include "ICommand.h"

#ifdef USE_IMGUI
#include "ImGuiManager.h"
#endif

#include "CollisionSystem.h"
#include "Random.h"

#include "Mesh.h"
#include "Material.h"

IMPLEMENT_SINGLETON(EngineCore)

EngineCore::EngineCore()
{
}

EngineCore::~EngineCore()
{
	Free();
}

HRESULT EngineCore::Ready_Engine(HWND hWnd)
{
	this->hWnd = hWnd;

	TimerMgr = TimerManager::Create();
	if (!TimerMgr)
		return E_FAIL;

	FrameMgr = FrameManager::Create();
	if (!FrameMgr)
		return E_FAIL;

	SceneMgr = SceneManager::Create();
	if (!SceneMgr)
		return E_FAIL;

	ResourceMgr = ResourceManager::Create();
	if (!ResourceMgr)
		return E_FAIL;

	SoundMgr = SoundManager::Create();
	if (!SoundMgr)
		return E_FAIL;

	RenderSys = RenderSystem::Create();
	if (!RenderSys)
		return E_FAIL;

	LightSys = LightSystem::Create();
	if (!LightSys)
		return E_FAIL;

	InputSys = InputSystem::Create();
	if (!InputSys)
		return E_FAIL;

	chunkLoader = ChunkLoader::Create();
	if (!chunkLoader)
		return E_FAIL;

	random = Random::Create();
	if (!random)
		return E_FAIL;

#ifdef USE_IMGUI
	ImGuiMgr = ImGuiManager::Create(hWnd);
	if (!ImGuiMgr)
		return E_FAIL;

	ImGuiMgr->RegisterWindow(L"Debug", [this]() {this->DebugSetting_IMGUI();});
#endif

	return S_OK;
}

void EngineCore::Tick(float dt)
{
	InputSys->BeginFrame();
	SoundMgr->Update();
#ifdef USE_IMGUI
	ImGuiMgr->BeginFrame();
#endif

	SceneMgr->Update(dt);
	SceneMgr->Late_Update(dt);

	LightSys->ApplyLight();

	RenderSys->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	RenderSys->Render();

#ifdef USE_IMGUI
	ImGuiMgr->Render();
#endif

	RenderSys->Render_End();
	InputSys->EndFrame();

	ExecuteCommand();
}

void EngineCore::LoadDefaultResource()
{

}

void EngineCore::RegisterCommand(ICommand* command)
{
	commands.push_back(command);
}

TimerManager* EngineCore::GetTimerManager() const
{
	return TimerMgr;
}

FrameManager* EngineCore::GetFrameManager() const
{
	return FrameMgr;
}

SceneManager* EngineCore::GetSceneManager() const
{
	return SceneMgr;
}

ResourceManager* EngineCore::GetResourceManager() const
{
	return ResourceMgr;
}

SoundManager* EngineCore::GetSoundManager() const
{
	return SoundMgr;
}

#ifdef USE_IMGUI
ImGuiManager* EngineCore::GetImGuiManager() const
{
	return ImGuiMgr;
}
#endif

RenderSystem* EngineCore::GetRenderSystem() const
{
	return RenderSys;
}

LightSystem* EngineCore::GetLightSystem() const
{
	return LightSys;
}

InputSystem* EngineCore::GetInputSystem() const
{
	return InputSys;
}

ChunkLoader* EngineCore::GetChunkLoader() const
{
	return chunkLoader;
}

Random* EngineCore::GetRandom() const
{
	return random;
}

HWND EngineCore::GetWindowHandle() const
{
	return hWnd;
}

#ifdef USE_IMGUI
void EngineCore::DebugSetting_IMGUI()
{
	ImGui::Begin("Debug Settings");

	if (ImGui::Button("Activate Debug Mode"))
		Debug_Mode = true;

	ImGui::SameLine();
	if (ImGui::Button("Deactivate Debug Mode"))
		Debug_Mode = false;

	// 현재 상태 표시
	ImGui::Text("Debug Mode is %s", Debug_Mode ? "ON" : "OFF");

	ImGui::End();
}
void EngineCore::ExecuteCommand()
{
	for (auto& command : commands)
	{
		command->Execute();
		Safe_Release(command);
	}
	commands.clear();
}
#endif

void EngineCore::Free()
{
	Safe_Release(FrameMgr);
	Safe_Release(TimerMgr);
	Safe_Release(SceneMgr);
	Safe_Release(ResourceMgr);
	Safe_Release(SoundMgr);
	Safe_Release(RenderSys);
	Safe_Release(LightSys);
	Safe_Release(InputSys);

#ifdef USE_IMGUI	
	Safe_Release(ImGuiMgr);
#endif
	GraphicDevice::GetInstance()->DestroyInstance();
}