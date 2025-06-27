#pragma once
#include "Scene.h"

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

private:
    void Free() override;
};