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
    using ChunkCoord = std::pair<int, int>;

private:
    ChunkManager(Scene* owner);
    virtual ~ChunkManager();

public:
    static ChunkManager* Create(Scene* owner);
    void Update(_float dt);

public:
    void SaveChunk(const std::wstring& saveStage);
    void LoadChunk(const std::wstring& loadStage);
    
    Chunk* CreateChunk(int chunkX, int chunkZ);

    const unordered_map<ChunkCoord, Chunk*, PairHash>& GetChunks() { return worldChunks; }

private:
    void Free()override;

private:
    Scene* owner = nullptr;
    std::unordered_map<ChunkCoord, Chunk*, PairHash> worldChunks;
};
END