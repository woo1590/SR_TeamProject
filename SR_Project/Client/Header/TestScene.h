#pragma once
#include "Scene.h"

class Player;

class TestScene : public Scene
{
private:
    explicit TestScene();
    virtual ~TestScene();

public:
    static TestScene* Create();

    void Load() override;
    void Update(float dt) override;
    void Late_Update(float dt) override;
    void Unload() override;


    /*---------ImGui Test---------*/
    void TestSceneImGui();
private:
    void Free() override;

private:
    Player* player;
};

