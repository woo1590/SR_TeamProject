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
class EventSystem;
class CollisionSystem;
class ChunkLoader;
class Random;

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
    SoundManager*       GetSoundManager()const;

#ifdef USE_IMGUI
    ImGuiManager*       GetImGuiManager()const;
#endif

    RenderSystem*       GetRenderSystem()const;
    LightSystem*        GetLightSystem()const;
    InputSystem*        GetInputSystem()const;

    ChunkLoader*        GetChunkLoader()const;
    Random* GetRandom()const;

    HWND GetWindowHandle()const;

#ifdef USE_IMGUI
    /*-----Debug Mode------*/
    void DebugSetting_IMGUI();
#endif
    _bool IsDebugMode()const { return Debug_Mode; }
    void SetDebugMode(bool debug) { Debug_Mode = debug; }
private:
    TimerManager* TimerMgr;
    FrameManager* FrameMgr;
    SceneManager* SceneMgr;
    ResourceManager* ResourceMgr;
    SoundManager* SoundMgr;
    Random* random;

#ifdef USE_IMGUI
    ImGuiManager* ImGuiMgr;
#endif

    RenderSystem* RenderSys;
    LightSystem* LightSys;
    InputSystem* InputSys;

    ChunkLoader* chunkLoader;

    HWND hWnd;

    _bool Debug_Mode = false;
    void Free()override;
};

END