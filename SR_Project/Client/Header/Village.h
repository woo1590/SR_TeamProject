#pragma once
#include "Scene.h"

class Village :
    public Scene
{
private:
    Village();
    virtual ~Village();

public:
    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

private:
    void Free()override;
    
};

