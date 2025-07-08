#pragma once
#include "Object.h"
#include "Engine_Struct.h"
#include "ObjectManager.h"

BEGIN(Engine)
class ChunkMesh;
class ENGINE_DLL Chunk : public Object
{
private:
    Chunk(ObjectManager* owner, int chunkX, int chunkZ);

public:
    virtual ~Chunk();

public:
    static Chunk * Create(ObjectManager* owner, int chunkX, int chunkZ);
    HRESULT Ready_Object()override;
    void AddBlock(const _vec3& pos, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage);

    void InitializeAirBlocks();
    void BuildChunkFace();
    void AddFace(std::vector<VTXTEX>& vertices, std::vector<uint32_t>& indices, const _vec3& blockPos, int faceDir);

    void SetBlock(int x, int y, int z, const StaticBlockData& block);
    void SetBlocksFromFlatVector(const std::vector<SB>& flatBlocks);

    int GetChunkX() const { return ChunkX; }
    int GetChunkZ() const { return ChunkZ; }
    StaticBlockData GetBlock(int x, int y, int z) const;

private:
    void Free()override;

private:
    int ChunkX = 0, ChunkZ = 0;
    SB Blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];

    ChunkMesh* mesh = nullptr;
};
END