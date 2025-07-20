#pragma once
#include "Scene.h"

class Player;
class Stage1 :
    public Scene
{
private:
    Stage1();
    virtual ~Stage1();

public:
    static Stage1* Create();

    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

    void DebugIMGUI();
private:
    void Free()override;

    Player* player;
};

