#pragma once
#include "Scene.h"

class Player;
class RedGolem;
class WayPointCam;
class Stage1 :
    public Scene
{
private:
    Stage1();
    virtual ~Stage1();

public:
    enum class Stage1State
    {
        Stage1Intro,
        BossIntro,
        Play
    };
    void ChangeState(Stage1State state);

public:
    static Stage1* Create();

    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

    void DebugIMGUI();
    void WayPointEdit();
private:
    void SetTriggerBox();
    void Free()override;

    Player* player;
    RedGolem* boss = nullptr;

    _float stage1IntroDuration = 0.f;
    _float stage1IntroTimer = 0.f;

    Stage1State currState = Stage1State::Play;

    WayPointCam* wayCam = nullptr;
};

