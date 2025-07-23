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

public:
    enum class Stage2Stage
    {
        Stage2Intro,
        BossIntro,
        ActiveBridge,
        Play
    };
    void ChangeState(Stage2Stage state);
public:
    static Stage2* Create();

    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

    void DebugIMGUI();
    void WayPointEdit();
private:
    void SetTriggerBox();
    void Free()override;

    Player* player = nullptr;

    WayPointCam* wayCam = nullptr;
    Stage2Stage currState = {};

    _float stage2IntroDuration = 0.f;
    _float stage2IntroTimer = 0.f;

    _float bridgeActiveDuration = 0.f;
    _float bridgeActiveTimer = 0.f;
    _bool isBridgeActive = false;
    _bool cameraShake = false;

    Object* boss = nullptr;

    std::vector<DynamicBlock*> levers;
};

