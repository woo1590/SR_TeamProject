#include "EnginePCH.h"
#include "Chunk.h"
#include "ChunkMesh.h"
#include "ObjectManager.h"
#include "CollisionBlock.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "ChunkManager.h"
#include "StaticBlock.h"
#include "Scene.h"

Chunk::Chunk(ObjectManager* owner, int chunkX, int chunkZ) : Object(owner,ObjectType::Chunk)
{
    ChunkX = chunkX;
    ChunkZ = chunkZ;
}

Chunk::~Chunk()
{
}

Chunk* Chunk::Create(ObjectManager* owner, int chunkX, int chunkZ)
{
    Chunk* Instance = new Chunk(owner, chunkX, chunkZ);

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
    transform->SetPosition(ChunkX * CHUNK_SIZE, 0.f, ChunkZ * CHUNK_SIZE);

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    renderer->SetMaterial("Chunk_Mtrl");
    renderer->SetMesh(mesh);
    
    owner->AddObject(ObjectType::Chunk, this);
    return S_OK;
}

void Chunk::AddBlock(const _vec3& position, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
{
    int localX = (static_cast<int>(position.x) - ChunkX * CHUNK_SIZE) / BLOCK_SIZE;
    int localY = static_cast<int>(position.y) / BLOCK_SIZE;
    int localZ = (static_cast<int>(position.z) - ChunkZ * CHUNK_SIZE) / BLOCK_SIZE;

    if (localY < 0 || localY >= CHUNK_HEIGHT / BLOCK_SIZE) return;
    if (localX < 0 || localX >= CHUNK_SIZE / BLOCK_SIZE) return;
    if (localZ < 0 || localZ >= CHUNK_SIZE / BLOCK_SIZE) return;

    auto& block = Blocks[localX][localY][localZ];
    block.Pos = position;
    block.Type = type;
    block.Axis = axis;
    block.Rot = rot;
    block.Usage = usage;
}

Chunk* Chunk::GetNeighborChunk(int x, int z)
{
    if (!owner->GetOwner()) return nullptr;
    if (!owner->GetOwner()->GetChunkManager()) return nullptr;

    int neighborChunkX = ChunkX + x;
    int neighborChunkZ = ChunkZ + z;

    auto& chunks = owner->GetOwner()->GetChunkManager()->GetChunks();
    auto it = chunks.find({ neighborChunkX, neighborChunkZ });

    if (it != chunks.end())
        return it->second;

    return nullptr;
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
            if (y < 0 || y >= CHUNK_HEIGHT) return true;
            if (x >= 0 && x < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE)
            {
                return Blocks[x][y][z].Type == StaticBlockType::Air;
            }
            //else
            //{
            //    int offsetX(0), offsetZ(0);
            //    int localX(x), localZ(z);
            //
            //    if (x < 0)
            //    {
            //        offsetX = -1;
            //        localX = CHUNK_SIZE - 1;
            //    }
            //    else if (x >= CHUNK_SIZE)
            //    {
            //        offsetX = 1;
            //        localX = 0;
            //    }
            //
            //    if (z < 0)
            //    {
            //        offsetZ = -1;
            //        localZ = CHUNK_SIZE - 1;
            //    }
            //    else if (z >= CHUNK_SIZE)
            //    {
            //        offsetZ = 1;
            //        localZ = 0;
            //    }
            //
            //    Chunk* neighborChunk = GetNeighborChunk(offsetX, offsetZ);
            //
            //    if (!neighborChunk)
            //        return true;
            //    return neighborChunk->GetBlock(localX, y, localZ).Type == StaticBlockType::Air;
            //}
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

                _vec3 pos = block.Pos -_vec3(ChunkX * CHUNK_SIZE, 0.f, ChunkZ * CHUNK_SIZE);
                if (IsAir(x, y + 1, z)) AddFace(vertices, indices, pos, FaceDir::Face_Top, block);
                if (IsAir(x, y - 1, z)) AddFace(vertices, indices, pos, FaceDir::Face_Bottom, block);
                if (IsAir(x + 1, y, z)) AddFace(vertices, indices, pos, FaceDir::Face_Right, block);
                if (IsAir(x - 1, y, z)) AddFace(vertices, indices, pos, FaceDir::Face_Left, block);
                if (IsAir(x, y, z + 1)) AddFace(vertices, indices, pos, FaceDir::Face_Front, block);
                if (IsAir(x, y, z - 1)) AddFace(vertices, indices, pos, FaceDir::Face_Behind, block);
            }
        }
    }

    //Safe_Release(mesh);

    mesh = ChunkMesh::Create();
    if (FAILED(mesh->Ready_Mesh(vertices, indices)))
    {
        Safe_Release(mesh);
        mesh = nullptr;
    }

    auto renderer = GetComponent<MeshRenderer>();
    renderer->SetMesh(mesh);
}

void Chunk::AddFace(std::vector<VTXTEX>& vertices, std::vector<uint32_t>& indices, const _vec3& blockPos, int faceDir, const SB& sb)
{
    static const _vec3 offsets[6][4] =
    {
        { {-1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f, -1.f}, {-1.f, 1.f, -1.f} },
        { {1.f, -1.f, 1.f}, {-1.f, -1.f, 1.f}, {-1.f, -1.f, -1.f}, {1.f, -1.f, -1.f} },
        { {-1.f, 1.f, 1.f}, {-1.f, 1.f, -1.f}, {-1.f, -1.f, -1.f}, {-1.f, -1.f, 1.f} },
        { {1.f, 1.f, -1.f}, {1.f, 1.f, 1.f}, {1.f, -1.f, 1.f}, {1.f, -1.f, -1.f} },
        { {1.f, 1.f, 1.f}, {-1.f, 1.f, 1.f}, {-1.f, -1.f, 1.f}, {1.f, -1.f, 1.f} },
        { {-1.f, 1.f, -1.f}, {1.f, 1.f, -1.f}, {1.f, -1.f, -1.f}, {-1.f, -1.f, -1.f} }
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

    SetUV(sb, faceDir);
    int startIndex = static_cast<int>(vertices.size());

    for (int i = 0; i < 4; ++i)
    {
        VTXTEX v;
        v.vPosition = blockPos + offsets[faceDir][i];
        v.vNormal = faceOffsets[faceDir];
        v.vTexUV = TexUVs[i];
        vertices.push_back(v);
    }

    indices.push_back(startIndex + 0);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);

    indices.push_back(startIndex + 0);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 3);
}

void Chunk::SetUV(const SB& sb, int faceDir)
{
    switch (sb.Type)
    {
    case Dirt:
        TexUVs[0] = { 0.f, 0.f };
        TexUVs[1] = { 0.125f, 0.f };
        TexUVs[2] = { 0.125f, 0.125f };
        TexUVs[3] = { 0.125f, 0.f };
        break;
    
    case WoodPlank:
        TexUVs[0] = { 0.25f, 0.25f };
        TexUVs[1] = { 0.375f, 0.25f };
        TexUVs[2] = { 0.375f, 0.375f };
        TexUVs[3] = { 0.25f, 0.375f };
        break;

    case Stone:
        TexUVs[0] = { 0.125f, 0.125f };
        TexUVs[1] = { 0.25f, 0.125f };
        TexUVs[2] = { 0.25f, 0.25f };
        TexUVs[3] = { 0.125f, 0.25f };
        break;

    case CobbleStone:
        TexUVs[0] = { 0.f, 0.125f };
        TexUVs[1] = { 0.125f, 0.125f };
        TexUVs[2] = { 0.125f, 0.25f };
        TexUVs[3] = { 0.f, 0.25f };
        break;

    case SmoothStone:
        TexUVs[0] = { 0.25f, 0.125f };
        TexUVs[1] = { 0.375f, 0.125f };
        TexUVs[2] = { 0.375f, 0.25f };
        TexUVs[3] = { 0.25f, 0.25f };
        break;
    
    case StoneBrick:
        TexUVs[0] = { 0.375f, 0.125f };
        TexUVs[1] = { 0.5f, 0.125f };
        TexUVs[2] = { 0.5f, 0.25f };
        TexUVs[3] = { 0.375f, 0.25f };
        break;
    
    case MossyStoneBrick:
        TexUVs[0] = { 0.5f, 0.125f };
        TexUVs[1] = { 0.625f, 0.125f };
        TexUVs[2] = { 0.625f, 0.25f };
        TexUVs[3] = { 0.5f, 0.25f };
        break;

    case GrassDirt:
        switch (faceDir)
        {
        case Face_Top:
            TexUVs[0] = { 0.25f, 0.f };
            TexUVs[1] = { 0.375f, 0.f };
            TexUVs[2] = { 0.375f, 0.125f };
            TexUVs[3] = { 0.25f, 0.125f };
            break;
        case Face_Bottom:
            TexUVs[0] = { 0.f, 0.f };
            TexUVs[1] = { 0.125f, 0.f };
            TexUVs[2] = { 0.125f, 0.125f };
            TexUVs[3] = { 0.f, 0.125f };
            break;
        default:
            TexUVs[0] = { 0.125f, 0.f };
            TexUVs[1] = { 0.25f, 0.f };
            TexUVs[2] = { 0.25f, 0.125f };
            TexUVs[3] = { 0.125f, 0.125f };
            break;
        }
        break;

    case Wood:
    {
        bool isRingFace(false);
        const float texSize(0.125f);
        const _vec2 ringTexStart{ 0.125f, 0.25f };
        const _vec2 sideTexStart{ 0.0f, 0.25f };

        switch (sb.Axis)
        {
        case sAX:
            if (faceDir == Face_Left || faceDir == Face_Right) isRingFace = true;
            break;
        case sAY:
            if (faceDir == Face_Top || faceDir == Face_Bottom) isRingFace = true;
            break;
        case sAZ:
            if (faceDir == Face_Front || faceDir == Face_Behind) isRingFace = true;
            break;
        }

        _vec2 uv = isRingFace ? ringTexStart : sideTexStart;
        TexUVs[0] = { uv.x + texSize, uv.y + texSize };
        TexUVs[1] = { uv.x, uv.y + texSize };
        TexUVs[2] = { uv.x, uv.y };
        TexUVs[3] = { uv.x + texSize, uv.y };
    }
    break;
    }
}

StaticBlockData Chunk::GetBlock(int x, int y, int z) const
{
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE) return SB{};
    
    return Blocks[x][y][z];
}

void Chunk::BuildCollisionBlock()
{
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

                if (IsAir(x, y + 1, z) || IsAir(x, y - 1, z) ||
                    IsAir(x + 1, y, z) || IsAir(x - 1, y, z) ||
                    IsAir(x, y, z + 1) || IsAir(x, y, z - 1))
                {
                    auto collisionBlock = CollisionBlock::Create(owner, ObjectType::StaticBlock);
                    collisionBlock->GetComponent<TransformComponent>()->SetPosition(block.Pos);
                    owner->AddObject(ObjectType::StaticBlock, collisionBlock);
                }
            }
        }
    }
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
        int localX = static_cast<int>(block.Pos.x) % CHUNK_SIZE / BLOCK_SIZE;
        int localY = static_cast<int>(block.Pos.y) / BLOCK_SIZE;
        int localZ = static_cast<int>(block.Pos.z) % CHUNK_SIZE / BLOCK_SIZE;

        if (localX < 0 || localX >= CHUNK_SIZE || localY < 0 || localY >= CHUNK_HEIGHT || localZ < 0 || localZ >= CHUNK_SIZE) continue;

        Blocks[localX][localY][localZ] = block;
    }   
}

void Chunk::Free()
{
    Safe_Release(mesh);
    Object::Free();
}