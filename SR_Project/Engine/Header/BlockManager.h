#pragma once

BEGIN(Engine)
class Scene;
class Object;
class ENGINE_DLL BlockManager : public Base
{
private:
    explicit BlockManager(Scene* owner);
    virtual ~BlockManager();

public:
    Scene* GetOwner()const;
    static BlockManager* Create(Scene* owner);

    void LoadTexture();

    void SaveStage(const char* saveStage);
    void LoadStage(const char* loadStage);

    void SaveChunk(const char* saveStage);
    void LoadChunk(const char* loadStage);

private:
    void Free()override;

private:
    Scene* owner = nullptr;
};
END