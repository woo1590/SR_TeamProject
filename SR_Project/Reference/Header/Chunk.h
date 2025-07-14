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
    static Chunk* Create(ObjectManager* owner, int chunkX, int chunkZ);

private:
    HRESULT Ready_Object()override;

public:
    void AddBlock(const _vec3& pos, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage);
    void RemoveAlpha(const _vec3& pos);
    void ClearAlpha();

    Chunk* GetNeighborChunk(int x, int z);

    void InitializeAirBlocks();

    void BuildChunkFace();

    void AddFace(std::vector<VTXTEX>& vertices, std::vector<uint32_t>& indices, const _vec3& blockPos, int faceDir, const SB& sb);
    void AddQuad(std::vector<VTXTEX>& vertices, std::vector<uint32_t>& indices, const _vec3& center, const _vec3& scale, const SB& sb, int faceDir, bool parts = false);

    void SetUV(const SB& sb, int faceDir, bool parts);
    void SetUVTile(int tileX, int tileY);
    void SetUVTile(int tileX, int tileY, int halfX, int halfY, int faceDir, StaticBlockUsage usage, bool parts = false);

    void SetBlock(int x, int y, int z, const StaticBlockData& block);
    void AddAlphaBlock(Object* alphaBlock) { AlphaBlocks.push_back(alphaBlock); }

    void SetBlocksFromFlatVector(const std::vector<SB>& flatBlocks);

    int GetChunkX() const { return ChunkX; }
    int GetChunkZ() const { return ChunkZ; }
    StaticBlockData GetBlock(int x, int y, int z) const;
    vector<Object*> GetAlphaBlocks() const { return AlphaBlocks; }

    void BuildCollisionBlock();
    void SetBlockAir(int x, int y, int z) { Blocks[x][y][z].Type = Air; }

private:
    void Free()override;

private:
    int ChunkX = 0, ChunkZ = 0;
    ChunkMesh* mesh = nullptr;

    _vec2 TexUVs[4]{};
    SB Blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    std::vector<Object*> AlphaBlocks;
};
END