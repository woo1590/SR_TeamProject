#pragma once
#include "Scene.h"

class Player;
class RedGolem;
class Stage1 :
    public Scene
{
private:
    Stage1();
    virtual ~Stage1();

    enum class Stage1State
    {
        Stage1Intro,
        BossIntro,
        Play
    };

public:
    static Stage1* Create();

    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

    void DebugIMGUI();
private:
    void SetTriggerBox();
    void ChangeState(Stage1State state);
    void Free()override;

    Player* player;
    RedGolem* boss = nullptr;
    Stage1State currState = Stage1State::Play;
};

