#pragma once
#include "Base.h"

BEGIN(Engine)
class Scene;
class Prefab;
class ENGINE_DLL PrefabManager : public Base
{
private:
    PrefabManager(Scene* owner);
    virtual ~PrefabManager();

public:
    static PrefabManager* Create(Scene* owner);

    bool SavePrefab(const std::string& key, const std::string& filepath);
    bool LoadPrefab(const std::string& key, const std::string& filepath);
    void LoadAllPrefabs(const std::string& folderPath);

    Prefab* GetPrefab(const std::string& key);
    const unordered_map<std::string, Prefab*>& GetMap() { return PrefabMap; }
    void AddPrefab(const std::string& key, Prefab* prefab);

private:
    void Free()override;

private:
    Scene* owner = nullptr;
    std::unordered_map<std::string, Prefab*> PrefabMap;
};
END