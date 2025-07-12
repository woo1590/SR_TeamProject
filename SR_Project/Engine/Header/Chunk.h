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
    // ���� �߰� (���� ��ǥ -> ���� ��ǥ)
    void AddBlock(const _vec3& pos, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage);

    // �̿��� ûũ ��ȯ
    Chunk* GetNeighborChunk(int x, int z);

    // ûũ �� ���ϵ� ����� �ʱ�ȭ
    void InitializeAirBlocks();

    // ����� �̿��� �鸸 �߷�����
    void BuildChunkFace();

    void AddFace(std::vector<VTXTEX>& vertices, std::vector<uint32_t>& indices, const _vec3& blockPos, int faceDir, const SB& sb);
    void AddBox(std::vector<VTXTEX>& vertices, std::vector<uint32_t>& indices, const _vec3& center, const _vec3& scale, const SB& sb, int faceDir);
    void AddQuad(std::vector<VTXTEX>& vertices, std::vector<uint32_t>& indices, const _vec3& center, const _vec3& scale, const SB& sb, int faceDir);

    void SetUV(const SB& sb, int faceDir);
    void SetUVTile(int tileX, int tileY);
    void SetUVTile(int tileX, int tileY, int halfX, int halfY, int faceDir, StaticBlockUsage usage);

    void SetBlock(int x, int y, int z, const StaticBlockData& block);
    void SetBlocksFromFlatVector(const std::vector<SB>& flatBlocks);

    int GetChunkX() const { return ChunkX; }
    int GetChunkZ() const { return ChunkZ; }
    StaticBlockData GetBlock(int x, int y, int z) const;

    void BuildCollisionBlock();
    void SetBlockAir(int x, int y, int z) { Blocks[x][y][z].Type = Air; }

private:
    void Free()override;

private:
    int ChunkX = 0, ChunkZ = 0;

    _vec2 TexUVs[4]{};

    ChunkMesh* mesh = nullptr;
    SB Blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
};
END