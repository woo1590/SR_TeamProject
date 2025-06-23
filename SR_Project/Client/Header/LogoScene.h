#pragma once
#include "Scene.h"

class LogoScene :
    public Scene
{
    friend class Scene;
private:
    explicit LogoScene();
    virtual ~LogoScene();

public:
    void Load()override;
    void Update(float dt)override;
    void Late_Update(float dt)override;
    void Unload()override;

private:
    void Free()override;
};

