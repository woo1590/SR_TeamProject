#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class Scene;
class ENGINE_DLL SceneManager :
    public Base
{
private:
    explicit SceneManager();
    virtual ~SceneManager();

public:
    static SceneManager* Create();

    HRESULT Ready_SceneManager();
    void Update(float dt);
    void Late_Update(float dt);

    Scene* GetActiveScene()const;
    void SetActiveScene(Scene* nextScene);
private:
    void Free()override;

    Scene* ActiveScene;
};

END