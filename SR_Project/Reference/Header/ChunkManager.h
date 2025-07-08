#pragma once
#include "Base.h"

BEGIN(Engine)
class Chunk;
class Scene;
class ENGINE_DLL ChunkManager : public Base
{
private:
    struct PairHash
    {
        size_t operator()(const std::pair<int, int>& key) const { return std::hash<int>()(key.first) ^ (std::hash<int>()(key.second) << 1); }
    };

private:
    ChunkManager(Scene* owner);
    virtual ~ChunkManager();

public:
    static ChunkManager* Create(Scene* owner);

public:
    void SaveChunk(const std::wstring& saveStage);
    void LoadChunk(const std::wstring& loadStage);
    
    Chunk* CreateChunk(int chunkX, int chunkZ);
    void RemoveChunk(int chunkX, int chunkZ);

    const unordered_map<std::pair<int, int>, Chunk*, PairHash>& GetChunks() { return worldChunks; }

private:
    void Free()override;

private:
    Scene* owner = nullptr;
    std::unordered_map<std::pair<int, int>, Chunk*, PairHash> worldChunks;
};
END