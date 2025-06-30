#pragma once
#include "Scene.h"

class Player;

class TestScene : public Scene
{
    friend class Scene;

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
    void ImGuiTestFunc();
private:
    void Free() override;

private:
    Player* player;
    void LoadBlock();

    std::vector<BlockData> Blocks;
};

