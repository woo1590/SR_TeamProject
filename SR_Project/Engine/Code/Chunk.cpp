#include "EnginePCH.h"
#include "Chunk.h"
#include "ChunkMesh.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"

Chunk::Chunk(int chunkX, int chunkZ)
{
    ChunkX = chunkX;
    ChunkZ = chunkZ;
}

Chunk::~Chunk()
{
}

Chunk* Chunk::Create(int chunkX, int chunkZ)
{
    Chunk* Instance = new Chunk(chunkX, chunkZ);
    return Instance;
}

void Chunk::AddBlock(const _vec3& pos, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
{
    int localX = static_cast<int>(pos.x) % CHUNK_SIZE;
    int localY = static_cast<int>(pos.y);
    int localZ = static_cast<int>(pos.z) % CHUNK_SIZE;

    if (localX < 0 || localX >= CHUNK_SIZE ||
        localY < 0 || localY >= CHUNK_HEIGHT ||
        localZ < 0 || localZ >= CHUNK_SIZE)
        return;

    StaticBlockData& block = Blocks[localX][localY][localZ];
    block.Pos = pos;
    block.Type = type;
    block.Axis = axis;
    block.Rot = rot;
    block.Usage = usage;
}

void Chunk::Render()
{
    if (!Mesh) return;
    Mesh->Draw();
}

void Chunk::InitializeAirBlocks()
{
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_HEIGHT; ++y)
        {
            for (int z = 0; z < CHUNK_SIZE; ++z)
            {
                Blocks[x][y][z].Type = StaticBlockType::Air;
                Blocks[x][y][z].Pos = _vec3
                (
                    static_cast<float>(x + ChunkX * CHUNK_SIZE),
                    static_cast<float>(y),
                    static_cast<float>(z + ChunkZ * CHUNK_SIZE)
                );
            }
        }
    }
}

void Chunk::BuildChunkFace()
{
    std::vector<VTXTEX> vertices;
    std::vector<int> indices;

    auto IsAir = [&](int x, int y, int z) -> bool
        {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE) return true;
        return Blocks[x][y][z].Type == StaticBlockType::Air;
        };

    for (int y = 0; y < CHUNK_HEIGHT; ++y)
    {
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            for (int x = 0; x < CHUNK_SIZE; ++x)
            {
                const auto& block = Blocks[x][y][z];
                if (block.Type == StaticBlockType::Air)
                    continue;

                const _vec3& pos = block.Pos;
                if (IsAir(x, y + 1, z)) AddFace(vertices, indices, pos, FaceDir::Face_Top);
                if (IsAir(x, y - 1, z)) AddFace(vertices, indices, pos, FaceDir::Face_Bottom);
                if (IsAir(x + 1, y, z)) AddFace(vertices, indices, pos, FaceDir::Face_Right);
                if (IsAir(x - 1, y, z)) AddFace(vertices, indices, pos, FaceDir::Face_Left);
                if (IsAir(x, y, z + 1)) AddFace(vertices, indices, pos, FaceDir::Face_Front);
                if (IsAir(x, y, z - 1)) AddFace(vertices, indices, pos, FaceDir::Face_Behind);
            }
        }
    }

    Safe_Release(Mesh);
    if (vertices.empty() || indices.empty()) return;

    Mesh = ChunkMesh::Create();
    if (FAILED(Mesh->Ready_Mesh(vertices, indices)))
    {
        Safe_Release(Mesh);
        Mesh = nullptr;
    }
}

void Chunk::AddFace(std::vector<VTXTEX>& vertices, std::vector<int>& indices, const _vec3& blockPos, int faceDir)
{
    static const _vec3 offsets[6][4] =
    {
        { { -1.f, 1.f, -1.f }, { 1.f, 1.f, -1.f }, { 1.f, 1.f, 1.f }, { -1.f, 1.f, 1.f } },         // +Y
        { { -1.f, -1.f, 1.f }, { 1.f, -1.f, 1.f }, { 1.f, -1.f, -1.f }, { -1.f, -1.f, -1.f } },     // -Y
        { { 1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f, -1.f } },         // +X
        { { -1.f, -1.f, 1.f }, { -1.f, -1.f, -1.f }, { -1.f, 1.f, -1.f }, { -1.f, 1.f, 1.f } },     // -X
        { { -1.f, -1.f, 1.f }, { 1.f, -1.f, 1.f }, { 1.f, 1.f, 1.f }, { -1.f, 1.f, 1.f } },         // +Z
        { { 1.f, -1.f, -1.f }, { -1.f, -1.f, -1.f }, { -1.f, 1.f, -1.f }, { 1.f, 1.f, -1.f } },     // -Z
    };

    static const _vec3 normals[6] =
    {
        {  0,  1,  0 },  // +Y
        {  0, -1,  0 },  // -Y
        {  1,  0,  0 },  // +X
        { -1,  0,  0 },  // -X
        {  0,  0,  1 },  // +Z
        {  0,  0, -1 },  // -Z
    };

    static const _vec2 uvs[4] = // 아틀라스 이미지 UV 지정
    {
        { 0.f, 0.f },
        { 1.f, 0.f },
        { 1.f, 1.f },
        { 0.f, 1.f }
    };

    int startIndex = static_cast<int>(vertices.size());

    for (int i = 0; i < 4; ++i)
    {
        VTXTEX v;
        v.vPosition = blockPos * 2.f + offsets[faceDir][i];
        v.vNormal = normals[faceDir];
        v.vTexUV = uvs[i];
        vertices.push_back(v);
    }

    indices.push_back(startIndex + 0);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);

    indices.push_back(startIndex + 0);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 3);
}


StaticBlockData Chunk::GetBlock(int x, int y, int z) const
{
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE) return SB{};

    return Blocks[x][y][z];
}

void Chunk::SetBlock(int x, int y, int z, const StaticBlockData& block)
{
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE) return;

    Blocks[x][y][z] = block;
}

void Chunk::SetBlocksFromFlatVector(const std::vector<SB>& flatBlocks)
{
    for (const auto& block : flatBlocks)
    {
        int localX = static_cast<int>(block.Pos.x) % CHUNK_SIZE;
        int localY = static_cast<int>(block.Pos.y);
        int localZ = static_cast<int>(block.Pos.z) % CHUNK_SIZE;

        if (localX < 0 || localX >= CHUNK_SIZE || localY < 0 || localY >= CHUNK_HEIGHT || localZ < 0 || localZ >= CHUNK_SIZE) continue;

        Blocks[localX][localY][localZ] = block;
    }
}

void Chunk::Free()
{
    memset(Blocks, 0, sizeof(Blocks));
    Safe_Release(Mesh);
}