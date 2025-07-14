#pragma once
#include "Scene.h"

class Stage2 :
    public Scene
{
private:
    Stage2();
    virtual ~Stage2();

public:
    static Stage2* Create();

    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

private:
    void Free()override;
};

