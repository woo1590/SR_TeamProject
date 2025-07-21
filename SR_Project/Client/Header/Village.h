#pragma once
#include "Scene.h"

class Player;
class Village :
    public Scene
{
private:
    Village();
    virtual ~Village();

    enum class VillageState
    {
        Intro,
        Play
    };

public:
    static Village* Create();

    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

    void DebugIMGUI();
private:
    void ChangeState(VillageState state);

    void Free()override;
    Player* player = nullptr;

    VillageState currState = {};

    _float introDuration = 5.f;
    _float introTimer = 0.f;
};

