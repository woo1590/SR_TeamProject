#pragma once
#include "Scene.h"
namespace Engine
{
    class DynamicBlock;
}

class Player;
class WayPointCam;
class Stage2 :
    public Scene
{
private:
    Stage2();
    virtual ~Stage2();

    enum class Stage2Stage
    {
        Stage2Intro,
        BossIntro,
        ActiveBridge,
        Play
    };

public:
    static Stage2* Create();

    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

    void DebugIMGUI();
    void WayPointEdit();
private:
    void ChangeState(Stage2Stage state);
    void Free()override;

    Player* player = nullptr;

    WayPointCam* wayCam = nullptr;
    Stage2Stage currState = {};

    _float stage2IntroDuration = 0.f;
    _float stage2IntroTimer = 0.f;

    _float bridgeActiveDuration = 0.f;
    _float bridgeActiveTimer = 0.f;
    _bool isBridgeActive = false;

    Object* boss = nullptr;

    std::vector<DynamicBlock*> levers;
};

