#pragma once
#include "Scene.h"

class Stage1 :
    public Scene
{
private:
    Stage1();
    virtual ~Stage1();

public:
    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

private:
    void Free()override;
};

