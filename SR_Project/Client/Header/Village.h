#pragma once
#include "Scene.h"

class Player;
class Village :
    public Scene
{
private:
    Village();
    virtual ~Village();

public:
    static Village* Create();

    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

    void DebugIMGUI();
private:
    void Free()override;
    Player* player = nullptr;

};

