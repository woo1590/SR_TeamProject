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
    void InitializeAirBlocks();
    void AddBlock(const _vec3& pos, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage);
    
    void ClearAlpha();
    void RemoveAlpha(const _vec3& pos);
    void AddAlphaBlock(Object* alphaBlock) { AlphaBlocks.push_back(alphaBlock); }

    Chunk* GetNeighborChunk(int x, int z);

    void BuildCollisionBlock();

    void BuildChunkFace();
    void AddFace(std::vector<VTXTEX>& vertices, std::vector<uint32_t>& indices, const _vec3& blockPos, int faceDir, const SB& sb);
    void AddQuad(std::vector<VTXTEX>& vertices, std::vector<uint32_t>& indices, const _vec3& center, const _vec3& scale, const SB& sb, int faceDir, bool parts = false);

    void SetUV(const SB& sb, int faceDir, bool parts);
    void SetUVTile(int tileX, int tileY);
    void SetUVTile(int tileX, int tileY, int halfX, int halfY, int faceDir, StaticBlockUsage usage, bool parts = false);
    void SetUVAxisBlock(StaticBlockType type, StaticBlockAxis axis, int faceDir);

    void SetBlock(int x, int y, int z, const StaticBlockData& block);
    void SetBlockAir(int x, int y, int z)
    {
        if (Blocks[x][y - 1][z].Type == DarkDirt && Blocks[x][y][z].Type == DarkGrass) Blocks[x][y - 1][z].Type = DarkGrass;
        Blocks[x][y][z].Type = Air;
    }
    void SetBlocksFromFlatVector(const std::vector<SB>& flatBlocks);

    void SetChunkRender(bool render);
    bool GetChunkRender();

    int GetChunkX() const { return ChunkX; }
    int GetChunkZ() const { return ChunkZ; }
    StaticBlockData GetBlock(int x, int y, int z) const;
    vector<Object*> GetAlphaBlocks() const { return AlphaBlocks; }

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