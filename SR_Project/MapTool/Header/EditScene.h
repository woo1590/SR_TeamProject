#pragma once
#include "Scene.h"

class EditScene : public Scene
{
    friend class Scene;

private:
    explicit EditScene();
    virtual ~EditScene();

public:
    static EditScene* Create();

    void Load() override;
    void Update(float dt) override;
    void Late_Update(float dt) override;
    void Unload() override;

private:
    void Free() override;
};