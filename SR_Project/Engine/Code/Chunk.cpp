#include "EnginePCH.h"
#include "Chunk.h"
#include "ChunkMesh.h"
#include "ObjectManager.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"

Chunk::Chunk(ObjectManager* owner, int chunkX, int chunkZ)
    :Object(owner,ObjectType::Chunk)
{
    ChunkX = chunkX;
    ChunkZ = chunkZ;
}

Chunk::~Chunk()
{
}

Chunk* Chunk::Create(ObjectManager* owner, int chunkX, int chunkZ)
{
    Chunk* Instance = new Chunk(owner,chunkX, chunkZ);

    if (FAILED(Instance->Ready_Object()))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Chunk::Ready_Object()
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(ChunkX * CHUNK_SIZE * 2.f, 0.f, ChunkZ * CHUNK_SIZE * 2.f);

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    renderer->SetMaterial("Chunk_Mtrl");
    renderer->SetMesh(mesh);
    
    owner->AddObject(ObjectType::Chunk, this);
    return S_OK;
}

void Chunk::AddBlock(const _vec3& pos, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
{
    int localX = static_cast<int>(pos.x / 2) % CHUNK_SIZE;
    int localY = static_cast<int>(pos.y / 2);
    int localZ = static_cast<int>(pos.z / 2) % CHUNK_SIZE;

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

void Chunk::InitializeAirBlocks()
{
    for (int y = 0; y < CHUNK_HEIGHT; ++y)
    {
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            for (int x = 0; x < CHUNK_SIZE; ++x)
            {
                Blocks[x][y][z].Type = StaticBlockType::Air;
            }
        }
    }
}

void Chunk::BuildChunkFace()
{
    std::vector<VTXTEX> vertices;
    std::vector<uint32_t> indices;

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

                // const _vec3& pos = block.Pos;
                _vec3 pos = block.Pos - _vec3(ChunkX * CHUNK_SIZE * 2.f, 0.f, ChunkZ * CHUNK_SIZE * 2.f);
                if (IsAir(x, y + 1, z)) AddFace(vertices, indices, pos, FaceDir::Face_Top);
                if (IsAir(x, y - 1, z)) AddFace(vertices, indices, pos, FaceDir::Face_Bottom);
                if (IsAir(x + 1, y, z)) AddFace(vertices, indices, pos, FaceDir::Face_Right);
                if (IsAir(x - 1, y, z)) AddFace(vertices, indices, pos, FaceDir::Face_Left);
                if (IsAir(x, y, z + 1)) AddFace(vertices, indices, pos, FaceDir::Face_Front);
                if (IsAir(x, y, z - 1)) AddFace(vertices, indices, pos, FaceDir::Face_Behind);
            }
        }
    }

    Safe_Release(mesh);
    if (vertices.empty() || indices.empty()) return;

    mesh = ChunkMesh::Create();
    if (FAILED(mesh->Ready_Mesh(vertices, indices)))
    {
        Safe_Release(mesh);
        mesh = nullptr;
    }

    auto renderer = GetComponent<MeshRenderer>();
    renderer->SetMesh(mesh);
}

void Chunk::AddFace(std::vector<VTXTEX>& vertices, std::vector<uint32_t>& indices, const _vec3& blockPos, int faceDir)
{
    static const _vec3 offsets[6][4] =
    {
        { {-1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, -1.f}, {-1.f, 1.f, -1.f} },
        { {-1.f, -1.f, 1.f}, {1.f, -1.f, 1.f}, {1.f, -1.f, -1.f}, {-1.f, -1.f, -1.f} },
        { {-1.f, 1.f, 1.f}, {-1.f, 1.f, -1.f}, {-1.f, -1.f, -1.f}, {-1.f, -1.f, 1.f} },
        { {1.f, 1.f, -1.f}, {1.f, 1.f, 1.f}, {1.f, -1.f, 1.f}, {1.f, -1.f, -1.f} },
        { {1.f, 1.f, 1.f}, {-1.f, 1.f, 1.f}, {-1.f, -1.f, 1.f}, {1.f, -1.f, 1.f} },
        { {-1.f, 1.f, -1.f}, {1.f, 1.f, -1.f}, {1.f, -1.f, -1.f}, {-1.f, -1.f, -1.f} }
        
        // { { -1.f, 1.f, -1.f }, { -1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f, -1.f } },        // +Y
        // { { -1.f, -1.f, -1.f }, { 1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f }, { -1.f, -1.f, 1.f } },     // -Y
        // { { 1.f, -1.f, -1.f }, { 1.f, -1.f, 1.f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f, -1.f } },         // +X
        // { { -1.f, -1.f, 1.f }, { -1.f, -1.f, -1.f }, { -1.f, 1.f, -1.f }, { -1.f, 1.f, 1.f } },     // -X
        // { { -1.f, -1.f, 1.f }, { 1.f, -1.f, 1.f }, { 1.f, 1.f, 1.f }, { -1.f, 1.f, 1.f } },         // +Z
        // { { 1.f, -1.f, -1.f }, { -1.f, -1.f, -1.f }, { -1.f, 1.f, -1.f }, { 1.f, 1.f, -1.f } },     // -Z
    };

    static const _vec3 faceOffsets[6] =
    {
        { 0.f, 1.f, 0.f },   // +Y (Top)
        { 0.f, -1.f, 0.f },  // -Y (Bottom)
        { 1.f, 0.f, 0.f },   // +X (Right)
        { -1.f, 0.f, 0.f },  // -X (Left)
        { 0.f, 0.f, 1.f },   // +Z (Front)
        { 0.f, 0.f, -1.f },  // -Z (Back)
    };

    static const _vec2 uvs[4] = // 아틀라스 이미지 UV 지정
    {
        { 0.f, 1.f },
        { 0.f, 0.f },
        { 1.f, 0.f },
        { 1.f, 1.f }
    };

    int startIndex = static_cast<int>(vertices.size());

    for (int i = 0; i < 4; ++i)
    {
        VTXTEX v;
        v.vPosition = blockPos + offsets[faceDir][i];
        v.vNormal = faceOffsets[faceDir];
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
        int blockX = static_cast<int>(floor(block.Pos.x / 2.f));
        int blockZ = static_cast<int>(floor(block.Pos.z / 2.f));

        int localX = blockX - ChunkX * CHUNK_SIZE;
        int localZ = blockZ - ChunkZ * CHUNK_SIZE;

        int localY = static_cast<int>(block.Pos.y / 2);

        if (localX < 0 || localX >= CHUNK_SIZE || localY < 0 || localY >= CHUNK_HEIGHT || localZ < 0 || localZ >= CHUNK_SIZE) continue;

        Blocks[localX][localY][localZ] = block;
    }
}

void Chunk::Free()
{
    memset(Blocks, 0, sizeof(Blocks));
}