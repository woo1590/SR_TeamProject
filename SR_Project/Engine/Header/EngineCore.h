#pragma once

BEGIN(Engine)

class TimerManager;
class FrameManager;
class SceneManager;
class RenderSystem;
class LightSystem;
class InputSystem;
class ResourceManager;
class UIManager;
class SoundManager;
class ImGuiManager;

class ENGINE_DLL EngineCore :
    public Base
{
    DECLARE_SINGLETON(EngineCore)
private:
    explicit EngineCore();
    virtual ~EngineCore();

public:
    HRESULT Ready_Engine(HWND hWnd);
    void Tick(float dt);

    /*---------------------*/
    void LoadDefaultResource();

    /*---------------------*/
    TimerManager*       GetTimerManager()const;
    FrameManager*       GetFrameManager()const;
    SceneManager*       GetSceneManager()const;
    ResourceManager*    GetResourceManager()const;
    UIManager*          GetUIManager() const;
    SoundManager*       GetSoundManager()const;
    ImGuiManager*       GetImGuiManager()const;

    RenderSystem*       GetRenderSystem()const;
    LightSystem*        GetLightSystem()const;
    InputSystem*        GetInputSystem()const;

    HWND GetWindowHandle()const;
private:
    TimerManager* TimerMgr;
    FrameManager* FrameMgr;
    SceneManager* SceneMgr;
    ResourceManager* ResourceMgr;
    SoundManager* SoundMgr;
    ImGuiManager* ImGuiMgr;

    RenderSystem* RenderSys;
    LightSystem* LightSys;
    InputSystem* InputSys;


    HWND hWnd;

    void Free()override;
};

END