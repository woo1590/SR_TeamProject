#pragma once
#include "Base.h"

class Player;
class GameManager :
    public Base
{
    DECLARE_SINGLETON(GameManager);
private:
    GameManager();
    virtual ~GameManager();

public:
    HRESULT Ready_GameManager();

    void ClearScene(LOADID sceneID);
    _bool IsSceneClear(LOADID sceneID);

    void SetPlayer(Player* player);
    Player* GetPlayer()const { return player; }
private:
    void Free()override;

    std::vector<_bool> clearScenes;
    Player* player = nullptr;
};

