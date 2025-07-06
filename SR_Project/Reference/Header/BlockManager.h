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
    static BlockManager* Create(Scene* owner);

    void LoadTexture();

    void SaveStage(const char* saveStage);   // ∏  ¿˙¿Â
    void LoadStage(const char* loadStage);   // ∏  ∑ŒµÂ

    void SaveChunk(const char* saveStage);
    void LoadChunk(const char* loadStage);

    Scene* GetOwner()const;

private:
    void Free()override;

private:
    Scene* owner = nullptr;
};
END