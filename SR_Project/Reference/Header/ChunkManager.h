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
    Chunk* CreateChunk(int chunkX, int chunkZ);     // Key 값의 청크 있으면 해당 청크 반환, 없으면 생성 후 반환
    void RemoveChunk(int chunkX, int chunkZ);       // Key 값의 청크 있으면 제거
    void ClearAllChunks();                          // 모든 청크 클리어 및 하나의 청크만 남기고 메모리 해제

    void SaveChunk(const std::wstring& saveStage);
    void LoadChunk(const std::wstring& loadStage, bool isEditor = false);

    Chunk* GetChunk(int chunkX, int chunkZ);        // Key 값의 청크 있으면 해당 청크 반환
    const unordered_map<std::pair<int, int>, Chunk*, PairHash>& GetChunks() { return worldChunks; }

private:
    void Free()override;

private:
    Scene* owner = nullptr;
    std::unordered_map<std::pair<int, int>, Chunk*, PairHash> worldChunks;
};
END