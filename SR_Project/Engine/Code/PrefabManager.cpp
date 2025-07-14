#include "EnginePCH.h"
#include "PrefabManager.h"
#include "EngineCore.h"
#include "Prefab.h"
#include "Scene.h"
#include "json.hpp"

using json = nlohmann::json;

PrefabManager::PrefabManager(Scene* owner) : owner(owner)
{
}

PrefabManager::~PrefabManager()
{
}

PrefabManager* PrefabManager::Create(Scene* owner)
{
	PrefabManager* Instance = new PrefabManager(owner);
	return Instance;
}

bool PrefabManager::SavePrefab(const std::string& key, const std::string& filepath)
{
    auto it = PrefabMap.find(key);
    if (it == PrefabMap.end()) return false;
    return it->second->SaveToFile(filepath);
}

bool PrefabManager::LoadPrefab(const std::string& key, const std::string& filepath)
{
    Prefab* prefab = new Prefab(key);
    if (!prefab->LoadFromFile(filepath))
    {
        Safe_Release(prefab);
        return false;
    }
    PrefabMap[key] = prefab;
    return true;
}

void PrefabManager::LoadAllPrefabs(const std::string& folderPath)
{
    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        if (entry.path().extension() == ".json")
        {
            std::string filename = entry.path().stem().string();
            LoadPrefab(filename, entry.path().string());
        }
    }
}

 Prefab* PrefabManager::GetPrefab(const std::string& key)
 {
 	auto it = PrefabMap.find(key);
 	if (it != PrefabMap.end()) return it->second;
 	return nullptr;
 }

 void PrefabManager::AddPrefab(const std::string& key, Prefab* prefab)
 {
 	PrefabMap[key] = prefab;
 }

void PrefabManager::Free()
{
	for (auto& [key, prefab] : PrefabMap)
		Safe_Release(prefab);
	PrefabMap.clear();
}