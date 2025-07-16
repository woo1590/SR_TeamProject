#include "EnginePCH.h"
#include "Chunk.h"
#include "ChunkMesh.h"
#include "ObjectManager.h"
#include "CollisionBlock.h"
#include "AlphaBlock.h"

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
    _vec3 chunkPos{ float(ChunkX * CHUNK_SIZE), 0.f, float(ChunkZ * CHUNK_SIZE) };
    transform->SetPosition(chunkPos);

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    renderer->SetMaterial("Chunk_Mtrl");
    renderer->SetMesh(mesh);

    owner->AddObject(ObjectType::Chunk, this);

    return S_OK;
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

void Chunk::AddBlock(const _vec3& position, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
{
    if (usage == Alpha)
    {
        auto block = StaticBlock::Create(owner, ObjectType::AlphaBlock, type, axis, rot, usage);
        block->GetComponent<TransformComponent>()->SetPosition(position);
        AlphaBlocks.push_back(block);
        owner->AddObject(ObjectType::AlphaBlock, block);
    }
    else
    {
        int localX = (static_cast<int>(position.x) - ChunkX * CHUNK_SIZE) / BLOCK_SIZE;
        int localY = static_cast<int>(position.y) / BLOCK_SIZE;
        int localZ = (static_cast<int>(position.z) - ChunkZ * CHUNK_SIZE) / BLOCK_SIZE;

        if (localY < 0 || localY >= CHUNK_HEIGHT / BLOCK_SIZE) return;
        if (localX < 0 || localX >= CHUNK_SIZE / BLOCK_SIZE) return;
        if (localZ < 0 || localZ >= CHUNK_SIZE / BLOCK_SIZE) return;

        SB block;
        block.Pos = position;
        block.Type = type;
        block.Axis = axis;
        block.Rot = rot;
        block.Usage = usage;

        Blocks[localX][localY][localZ] = block;
    }
}

void Chunk::ClearAlpha()
{
    for (auto& iter : AlphaBlocks)
        Safe_Release(iter);
    AlphaBlocks.clear();
}

void Chunk::RemoveAlpha(const _vec3& pos)
{
    for (auto iter = AlphaBlocks.begin(); iter != AlphaBlocks.end();)
    {
        Object* obj = *iter;
        if (obj->GetComponent<TransformComponent>()->GetPosition() == pos)
        {
            iter = AlphaBlocks.erase(iter);
            owner->RemoveObject(ObjectType::AlphaBlock, pos);
        }
        else ++iter;
    }
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

void Chunk::BuildCollisionBlock()
{
    auto IsAir = [&](int x, int y, int z) -> bool
        {
            if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE) return true;
            return Blocks[x][y][z].Type == StaticBlockType::Air;
        };

    for (int y = 0; y < 20; ++y)
    {
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            for (int x = 0; x < CHUNK_SIZE; ++x)
            {
                const auto& block = Blocks[x][y][z];
                if (block.Type == StaticBlockType::Air) continue;

                _vec3 pos = block.Pos - _vec3(ChunkX * CHUNK_SIZE * 2.f, 0.f, ChunkZ * CHUNK_SIZE * 2.f);
                if (IsAir(x, y + 1, z) || IsAir(x, y - 1, z) ||
                    IsAir(x + 1, y, z) || IsAir(x - 1, y, z) ||
                    IsAir(x, y, z + 1) || IsAir(x, y, z - 1))
                {
                    switch (block.Usage)
                    {
                    case Basic:
                    {
                        auto cb = CollisionBlock::Create(owner, ObjectType::CollisionBlock);
                        cb->GetComponent<TransformComponent>()->SetPosition(block.Pos);
                        owner->AddObject(ObjectType::CollisionBlock, cb);

                        break;
                    }
                    case Half:
                    {
                        auto cb = CollisionBlock::Create(owner, ObjectType::CollisionBlock);
                        cb->GetComponent<TransformComponent>()->SetPosition(block.Pos + _vec3(0.f, -0.5f, 0.f));
                        cb->GetComponent<TransformComponent>()->SetScale(_vec3(1.f, 0.5f, 1.f));
                        owner->AddObject(ObjectType::CollisionBlock, cb);

                        break;
                    }
                    case Stair:
                    {
                        auto cbBottom = CollisionBlock::Create(owner, ObjectType::StaticBlock);
                        cbBottom->GetComponent<TransformComponent>()->SetPosition(block.Pos + _vec3(0.f, -0.5f, 0.f));
                        cbBottom->GetComponent<TransformComponent>()->SetScale(_vec3(1.f, 0.5f, 1.f));
                        owner->AddObject(ObjectType::StaticBlock, cbBottom);

                        _vec3 offset{}, scale{};
                        switch (block.Rot)
                        {
                        case sZP:
                            offset = { 0.f, 0.5f, 0.5f };
                            scale = { 1.f, 0.5f, 0.5f };
                            break;
                        case sZM:
                            offset = { 0.f, 0.5f, -0.5f };
                            scale = { 1.f, 0.5f, 0.5f };
                            break;
                        case sXP:
                            offset = { 0.5f, 0.5f, 0.f };
                            scale = { 0.5f, 0.5f, 1.f };
                            break;
                        case sXM:
                            offset = { -0.5f, 0.5f, 0.f };
                            scale = { 0.5f, 0.5f, 1.f };
                            break;
                        }

                        auto cbTop = CollisionBlock::Create(owner, ObjectType::StaticBlock);
                        cbTop->GetComponent<TransformComponent>()->SetPosition(block.Pos + offset);
                        cbTop->GetComponent<TransformComponent>()->SetScale(scale);
                        owner->AddObject(ObjectType::StaticBlock, cbTop);
                        break;
                    }
                    }
                }
            }
        }
    }
}

void Chunk::BuildChunkFace()
{
    std::vector<VTXTEX> vertices;
    std::vector<uint32_t> indices;

    auto IsFaceExposed = [&](int x, int y, int z, FaceDir faceDir, const SB& currentBlock) -> bool
        {
            const SB* neighborBlock(nullptr);
            if (x < 0 || x >= CHUNK_SIZE / BLOCK_SIZE || z < 0 || z >= CHUNK_SIZE / BLOCK_SIZE)
            {
                int offsetX(0), offsetZ(0);
                int localX(x), localZ(z);

                if (x < 0) { offsetX = -1; localX = CHUNK_SIZE / BLOCK_SIZE - 1; }
                else if (x >= CHUNK_SIZE / BLOCK_SIZE) { offsetX = 1; localX = 0; }

                if (z < 0) { offsetZ = -1; localZ = CHUNK_SIZE / BLOCK_SIZE - 1; }
                else if (z >= CHUNK_SIZE / BLOCK_SIZE) { offsetZ = 1; localZ = 0; }

                Chunk* neighborChunk = GetNeighborChunk(offsetX, offsetZ);
                if (!neighborChunk || y < 0 || y >= CHUNK_HEIGHT / BLOCK_SIZE) return true;
                neighborBlock = &neighborChunk->Blocks[localX][y][localZ];
            }
            else
            {
                if (y < 0 || y >= CHUNK_HEIGHT / BLOCK_SIZE) return true;
                neighborBlock = &Blocks[x][y][z];
            }

            const SB& neighbor(*neighborBlock);
            if (neighbor.Type == Air) return true;
            if (neighbor.Usage == Half && currentBlock.Usage == Half)
            {
                if (faceDir == Face_Top) return true;
                return false;
            }

            switch (currentBlock.Usage)
            {
            case Basic:
                if (neighbor.Usage == Basic) return false;
                if (neighbor.Usage == Half)
                {
                    if (faceDir == Face_Top) return false;
                    else return true;
                }
                if (neighbor.Usage == Stair)
                {
                    if (faceDir == Face_Top) return false;

                    bool neighborBackFace(false);
                    switch (neighbor.Rot)
                    {
                    case sZP: neighborBackFace = (faceDir == Face_Behind); break;
                    case sZM: neighborBackFace = (faceDir == Face_Front); break;
                    case sXP: neighborBackFace = (faceDir == Face_Left); break;
                    case sXM: neighborBackFace = (faceDir == Face_Right); break;
                    }
                    if (neighborBackFace) return false;
                    return true;
                }
                break;
            case Half:
                switch (faceDir)
                {
                case Face_Top: return currentBlock.Usage == Half;
                case Face_Bottom: return false;
                default: return false;
                }
                break;
            case Stair:
            {
                if (neighbor.Usage == Half) return true;
                if (neighbor.Usage == Basic)
                {
                    if (faceDir == Face_Bottom) return false;
                    switch (currentBlock.Rot)
                    {
                    case sZP: if (faceDir != Face_Behind) return false; break;
                    case sZM: if (faceDir != Face_Front) return false; break;
                    case sXP: if (faceDir != Face_Left) return false; break;
                    case sXM: if (faceDir != Face_Right) return false; break;
                    }
                    return true;
                }
                if (neighbor.Usage == Stair)
                {
                    if (currentBlock.Rot == neighbor.Rot)
                    {
                        if ((currentBlock.Rot == sZP || currentBlock.Rot == sZM) && (faceDir == Face_Right || faceDir == Face_Left)) return false;
                        if ((currentBlock.Rot == sXP || currentBlock.Rot == sXM) && (faceDir == Face_Front || faceDir == Face_Behind)) return false;
                    }

                    if ((neighbor.Rot == sXP && currentBlock.Rot == sXM && faceDir == Face_Left) ||
                        (neighbor.Rot == sXM && currentBlock.Rot == sXP && faceDir == Face_Right) ||
                        (neighbor.Rot == sZP && currentBlock.Rot == sZM && faceDir == Face_Behind) ||
                        (neighbor.Rot == sZM && currentBlock.Rot == sZP && faceDir == Face_Front))
                        return false;
                }
                return true;
            }
            }
            return true;
        };

    for (int y = 0; y < CHUNK_HEIGHT; ++y)
    {
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            for (int x = 0; x < CHUNK_SIZE; ++x)
            {
                const auto& block = Blocks[x][y][z];
                if (block.Type == StaticBlockType::Air) continue;

                _vec3 pos = block.Pos -_vec3(ChunkX * CHUNK_SIZE, 0.f, ChunkZ * CHUNK_SIZE);
                if (IsFaceExposed(x, y + 1, z, Face_Top, block)) AddFace(vertices, indices, pos, Face_Top, block);
                if (IsFaceExposed(x, y - 1, z, Face_Bottom, block)) AddFace(vertices, indices, pos, Face_Bottom, block);
                if (IsFaceExposed(x + 1, y, z, Face_Right, block)) AddFace(vertices, indices, pos, Face_Right, block);
                if (IsFaceExposed(x - 1, y, z, Face_Left, block)) AddFace(vertices, indices, pos, Face_Left, block);
                if (IsFaceExposed(x, y, z + 1, Face_Front, block)) AddFace(vertices, indices, pos, Face_Front, block);
                if (IsFaceExposed(x, y, z - 1, Face_Behind, block)) AddFace(vertices, indices, pos, Face_Behind, block);
            }
        }
    }

    Safe_Release(mesh);
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
    if (sb.Usage == Stair)
    {
        _vec3 bottomScale = { 1.f, 0.5f, 1.f }, bottomOffset = { 0.f, -0.5f, 0.f };
        AddQuad(vertices, indices, blockPos + bottomOffset, bottomScale, sb, faceDir);

        _vec3 topScale, topOffset;
        switch (sb.Rot)
        {
        case sZP:
            topScale = { 1.f, 0.5f, 0.5f };
            topOffset = { 0.f, 0.5f, 0.5f };
            break;
        case sZM:
            topScale = { 1.f, 0.5f, 0.5f };
            topOffset = { 0.f, 0.5f, -0.5f };
            break;
        case sXP:
            topScale = { 0.5f, 0.5f, 1.f };
            topOffset = { 0.5f, 0.5f, 0.f };
            break;
        case sXM:
            topScale = { 0.5f, 0.5f, 1.f };
            topOffset = { -0.5f, 0.5f, 0.f };
            break;
        }

        AddQuad(vertices, indices, blockPos + topOffset, topScale, sb, faceDir);
        return;
    }

    if (sb.Usage == Fence)
    {
        bool isCon(true);
        _vec3 stickScale = { 0.25f, 1.f, 0.25f }, stickOffset = { 0.f, 0.f, 0.f };
        AddQuad(vertices, indices, blockPos + stickOffset, stickScale, sb, faceDir, TRUE);

        _vec3 conScale, conOffset;
        switch (sb.Rot)
        {
        case sZP: case sZM:
            conScale = { 0.2f, 0.2f, 1.f };
            conOffset = { 0.f, 0.5f, 0.f };
            break;
        case sXP: case sXM:
            conScale = { 1.f, 0.2f, 0.2f };
            conOffset = { 0.f, 0.5f, 0.f };
            break;
        case sREnd:
            isCon = false;
            break;
        }

        if (!isCon) return;
        _vec3 offset{ 0.f, 0.8f, 0.f };
        AddQuad(vertices, indices, blockPos + conOffset, conScale, sb, faceDir);
        AddQuad(vertices, indices, blockPos + conOffset - offset, conScale, sb, faceDir);
        return;
    }

    float scaleY(1.f);
    _vec3 offsetY{ 0.f, 0.f, 0.f };

    if (sb.Usage == Half)
    {
        scaleY = 0.5f;
        offsetY = { 0.f, -0.5f, 0.f };
    }

    if (sb.Usage == Door)
    {
        _vec3 scale = { 1.f, 2.f, 0.1f };
        _vec3 offset = { 0.f, 1.f, 0.f };
        AddQuad(vertices, indices, blockPos + offset, scale, sb, faceDir, TRUE);
        return;
    }

    _vec3 scale{ 1.f, scaleY, 1.f };
    _vec3 center(blockPos + offsetY);
    AddQuad(vertices, indices, center, scale, sb, faceDir);
}

void Chunk::AddQuad(std::vector<VTXTEX>& vertices, std::vector<uint32_t>& indices, const _vec3& center, const _vec3& scale, const SB& sb, int faceDir, bool parts)
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
        { 0.f, 1.f, 0.f },
        { 0.f, -1.f, 0.f },
        { -1.f, 0.f, 0.f },
        { 1.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f },
        { 0.f, 0.f, -1.f },
    };

    SetUV(sb, faceDir, parts);
    int startIndex = static_cast<int>(vertices.size());

    for (int i = 0; i < 4; ++i)
    {
        _vec3 vertexOffset = offsets[faceDir][i];
        vertexOffset.x *= scale.x;
        vertexOffset.y *= scale.y;
        vertexOffset.z *= scale.z;

        VTXTEX v;
        v.vPosition = center + vertexOffset;
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

void Chunk::SetUV(const SB& sb, int faceDir, bool parts)
{
    switch (sb.Type)
    {
    case Dirt:
        SetUVTile(0, 0);
        break;
    case WoodPlank:
        if (sb.Usage == Half) SetUVTile(2, 2, 0, 3, faceDir, sb.Usage);
        else if (sb.Usage == Fence) SetUVTile(4, 2, 5, 2, faceDir, sb.Usage, parts);
        else if (sb.Usage == Door) SetUVTile(6, 2, 7, 2, faceDir, sb.Usage, parts);
        else SetUVTile(2, 2);
        break;
    case DarkWoodPlank:
        SetUVTile(1, 6);
        break;
    case Stone:
        if (sb.Usage == Half) SetUVTile(1, 1, 2, 3, faceDir, Half);
        else SetUVTile(1, 1);
        break;
    case CobbleStone:
        if (sb.Usage == Half) SetUVTile(0, 1, 3, 3, faceDir, Half);
        else SetUVTile(0, 1);
        break;
    case SmoothStone:
        if (sb.Usage == Half) SetUVTile(2, 1, 1, 3, faceDir, Half);
        else SetUVTile(2, 1);
        break;
    case StoneBrick:
        SetUVTile(3, 1);
        break;
    case MossyStoneBrick:
        SetUVTile(4, 1);
        break;
    case GrassDirt:
        switch (faceDir)
        {
        case Face_Top:
            SetUVTile(2, 0);
            break;
        case Face_Bottom:
            SetUVTile(0, 0);
            break;
        default:
            SetUVTile(1, 0);
            break;
        }
        break;
    case DirtPath:
        switch (faceDir)
        {
        case Face_Top:
            SetUVTile(4, 0);
            break;
        case Face_Bottom:
            SetUVTile(0, 0);
            break;
        default:
            SetUVTile(3, 0);
            break;
        }
        break;
    case Oak:
        SetUVTile(3, 2);
        break;
    case Furnace:
        switch (faceDir)
        {
        case Face_Top: case Face_Bottom:
            SetUVTile(2, 4);
            break;
        case Face_Front:
            if (sb.Rot == sZP) SetUVTile(0, 4);
            else SetUVTile(1, 4);
            break;
        case Face_Behind:
            if (sb.Rot == sZM) SetUVTile(0, 4);
            else SetUVTile(1, 4);
            break;
        case Face_Right:
            if (sb.Rot == sXP) SetUVTile(0, 4);
            else SetUVTile(1, 4);
            break;
        case Face_Left:
            if (sb.Rot == sXM) SetUVTile(0, 4);
            else SetUVTile(1, 4);
            break;
        }
        break;
    case Haybale: case Wood:
        SetUVAxisBlock(sb.Type, sb.Axis, faceDir);
        break;
    case WhiteWool:
        SetUVTile(2, 5);
        break;
    case YellowWool:
        SetUVTile(3, 5);
        break;
    case Terracota:
        SetUVTile(0, 6);
        break;
    }
}

void Chunk::SetUVTile(int tileX, int tileY)
{
    float tileSize(0.125f);
    float u(tileX * tileSize), v(tileY * tileSize);

    TexUVs[0] = { u, v };
    TexUVs[1] = { u + tileSize, v };
    TexUVs[2] = { u + tileSize, v + tileSize };
    TexUVs[3] = { u, v + tileSize };
}

void Chunk::SetUVTile(int tileX, int tileY, int halfX, int halfY, int faceDir, StaticBlockUsage usage, bool scale)
{
    float tileSize(0.125f);
    float halfSize(0.0625f);
    float quarterSize(0.03125f);
    float u(tileX * tileSize);
    float v(tileY * tileSize);

    if (usage == Half && (faceDir == Face_Left || faceDir == Face_Right || faceDir == Face_Front || faceDir == Face_Behind))
    {
        u = halfX * tileSize;
        v = halfY * tileSize;

        TexUVs[0] = { u, v };
        TexUVs[1] = { u + tileSize, v };
        TexUVs[2] = { u + tileSize, v + halfSize };
        TexUVs[3] = { u, v + halfSize };
    }
    else if (usage == Fence)
    {
        if (scale)
        {
            u = tileX * tileSize;
            v = tileY * tileSize;

            TexUVs[0] = { u, v };
            TexUVs[1] = { u + quarterSize, v };
            TexUVs[2] = { u + quarterSize, v + tileSize };
            TexUVs[3] = { u, v + tileSize };
        }
        else
        {
            u = halfX * tileSize;
            v = halfY * tileSize;

            TexUVs[0] = { u, v };
            TexUVs[1] = { u + tileSize, v };
            TexUVs[2] = { u + tileSize, v + quarterSize };
            TexUVs[3] = { u, v + quarterSize };
        }
    }
    else if (usage == Door)
    {
        if (faceDir == Face_Top || faceDir == Face_Bottom || faceDir == Face_Left || faceDir == Face_Right)
        {
            u = halfX * tileSize;
            v = halfY * tileSize;

            TexUVs[0] = { u, v };
            TexUVs[1] = { u + quarterSize, v };
            TexUVs[2] = { u + quarterSize, v + tileSize };
            TexUVs[3] = { u, v + tileSize };
        }
        else
        {
            TexUVs[0] = { u, v };
            TexUVs[1] = { u + tileSize, v };
            TexUVs[2] = { u + tileSize, v + tileSize };
            TexUVs[3] = { u, v + tileSize };
        }
    }

    else
    {
        TexUVs[0] = { u, v };
        TexUVs[1] = { u + tileSize, v };
        TexUVs[2] = { u + tileSize, v + tileSize };
        TexUVs[3] = { u, v + tileSize };
    }
}

void Chunk::SetUVAxisBlock(StaticBlockType type, StaticBlockAxis axis, int faceDir)
{
    bool isTop(false), isSide(false);
    float texSize(0.125f);

    _vec2 uvStart;
    _vec2 haybaleSideTex{ 0.f, 0.625f }, haybaleTopTex{ 0.125f, 0.625f };
    _vec2 woodSideTex{ 0.f, 0.25f }, woodRingTex{ 0.125f, 0.25f };

    switch (type)
    {
    case Haybale:
        switch (axis)
        {
        case sAX:
            isSide = true;
            if (faceDir == Face_Left || faceDir == Face_Right) isTop = true;
            break;
        case sAY:
            if (faceDir == Face_Top || faceDir == Face_Bottom) isTop = true;
            break;
        case sAZ:
            if (faceDir == Face_Front || faceDir == Face_Behind) isTop = true;
            if (faceDir == Face_Left || faceDir == Face_Right) isSide = true;
            break;
        }
        uvStart = isTop ? haybaleTopTex : haybaleSideTex;
        break;
    case Wood:
        switch (axis)
        {
        case sAX:
            isSide = true;
            if (faceDir == Face_Left || faceDir == Face_Right) isTop = true;
            break;
        case sAY:
            if (faceDir == Face_Top || faceDir == Face_Bottom) isTop = true;
            break;
        case sAZ:
            if (faceDir == Face_Front || faceDir == Face_Behind) isTop = true;
            if (faceDir == Face_Left || faceDir == Face_Right) isSide = true;
            break;
        }
        uvStart = isTop ? woodRingTex : woodSideTex;
        break;
    }

    if (!isTop && isSide)
    {
        TexUVs[0] = { uvStart.x, uvStart.y + texSize };
        TexUVs[1] = { uvStart.x, uvStart.y };
        TexUVs[2] = { uvStart.x + texSize, uvStart.y };
        TexUVs[3] = { uvStart.x + texSize, uvStart.y + texSize };
    }
    else
    {
        TexUVs[0] = { uvStart.x + texSize, uvStart.y + texSize };
        TexUVs[1] = { uvStart.x, uvStart.y + texSize };
        TexUVs[2] = { uvStart.x, uvStart.y };
        TexUVs[3] = { uvStart.x + texSize, uvStart.y };
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

void Chunk::SetChunkRender(bool render)
{
    mesh->SetRender(render);
}

bool Chunk::GetChunkRender()
{
    return mesh->GetRender();
}

StaticBlockData Chunk::GetBlock(int x, int y, int z) const
{
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE) return SB{};
    return Blocks[x][y][z];
}

void Chunk::Free()
{
    Safe_Release(mesh);

    //for (auto& alpha : AlphaBlocks)
    //    Safe_Release(alpha);

    Object::Free();
}