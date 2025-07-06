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
    void Activate(int chunkX, int chunkY);          // 청크 렌더링 활성화
    void UnLoadChunk(int chunkX, int chunkY);       // 메모리에 남아있는 청크 삭제

    void SaveChunk(const std::wstring& saveStage);
    void LoadChunk(const std::wstring& loadStage);

private:
    void Free()override;

private:
    Scene* owner = nullptr;
    std::unordered_map<std::pair<int, int>, Chunk*, PairHash> worldChunks;
};
END