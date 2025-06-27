#include "EnginePCH.h"
#include "EngineCore.h"
#include "GraphicDevice.h"
#include "FrameManager.h"
#include "TimerManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "LightSystem.h"
#include "InputSystem.h"
#include "SoundManager.h"
#include "ImGuiManager.h"

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

	UIMgr = UIManager::Create();
	if (!UIMgr)
		return E_FAIL;

	ImGuiMgr = ImGuiManager::Create(hWnd);

	return S_OK;
}

void EngineCore::Tick(float dt)
{
	InputSys->BeginFrame();
	ImGuiMgr->BeginFrame();

	SceneMgr->Update(dt);
	SceneMgr->Late_Update(dt);

	LightSys->ApplyLight();

	RenderSys->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	RenderSys->Render();
	RenderSys->Render_End();

	ImGuiMgr->EndFrame();
	InputSys->EndFrame();
}

void EngineCore::LoadDefaultResource()
{
	
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

ImGuiManager* EngineCore::GetImGuiManager() const
{
	return ImGuiMgr;
}

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

UIManager* EngineCore::GetUIManager() const
{
	return UIMgr;
}

HWND EngineCore::GetWindowHandle() const
{
	return hWnd;
}

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
	Safe_Release(UIMgr);
	Safe_Release(ImGuiMgr);

	GraphicDevice::GetInstance()->DestroyInstance();
}
