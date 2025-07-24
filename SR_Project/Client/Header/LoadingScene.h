#pragma once
#include "Scene.h"

class Loader;
class LoadingUI;
class Cursor;
class LoadingStone;
class Overlay;
class LoadingScene :
    public Scene
{
private:
    LoadingScene(LOADID loadId);
    virtual ~LoadingScene();

public:
    static LoadingScene* Create(LOADID loadId);

    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

private:
    void Free()override;

    Loader* loader = nullptr;
    LOADID nextSceneID;

    LoadingUI* loadingUI = nullptr;
    Cursor* cursor = nullptr;
    LoadingStone* loadingStone = nullptr;

    bool isLoadComplete = false;

    Overlay* overlay = nullptr;
};

