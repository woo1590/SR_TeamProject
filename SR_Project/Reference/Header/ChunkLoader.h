#pragma once
#include "Base.h"
#include "ChunkManager.h"

BEGIN(Engine)

class Object;
class Chunk;
class ENGINE_DLL ChunkLoader :
    public Base
{
private:
    ChunkLoader();
    virtual ~ChunkLoader();

public:
    static ChunkLoader* Create();
    HRESULT Ready_ChunkLoader();
    HRESULT LoadChunk(const std::wstring & loadPath);
    HRESULT LoadDB(const std::wstring & loadPath);

    std::unordered_map<std::pair<int, int>, Chunk*, ChunkManager::PairHash>& GetChunks() { return worldChunks; }
private:
    void Free()override;
     
    std::unordered_map<std::pair<int, int>, Chunk*, ChunkManager::PairHash> worldChunks;
    std::vector<StaticBlockData> staticBlocks;
};

END