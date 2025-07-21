#pragma once
#include "Scene.h"

class Player;
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

    void DebugIMGUI();
private:
    void Free()override;

    Player* player = nullptr;
};

