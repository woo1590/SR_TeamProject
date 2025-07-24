#pragma once
#include "Scene.h"

namespace Engine
{
    class DynamicBlock;
}

class Player;
class WayPointCam;
class Village :
    public Scene
{
private:
    Village();
    virtual ~Village();

public:
    enum class VillageState
    {
        EnterVillage,
        Play
    };
    void ChangeState(VillageState state);

public:
    static Village* Create();

    void Load()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void Unload()override;

    void DebugIMGUI();
    void WaypointEdit();
private:
    void SetTriggerBox();
    void Free()override;
    Player* player = nullptr;

    VillageState currState = {};

    _float introDuration = 10.f;
    _float introTimer = 0.f;
    _bool isEnterVillage = false;

    WayPointCam* wayCam = nullptr;

    std::vector<DynamicBlock*> levers;
};

