#include "EnginePCH.h"
#include "MinimapData.h"
#include "ChunkManager.h"
#include "Chunk.h"
#include "MinimapMesh.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "ObjectManager.h"

MinimapData::MinimapData(ObjectManager* owner) : Object(owner, ObjectType::UI)
{
}

MinimapData::~MinimapData()
{
    Safe_Release(mesh);
}

MinimapData* MinimapData::Create(ObjectManager* owner)
{
    MinimapData* instance = new MinimapData(owner);
    if (FAILED(instance->Ready_Object()))
    {
        Safe_Release(instance);
        return nullptr;
    }
    return instance;
}

HRESULT MinimapData::Ready_Object()
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition({ -10.f, 10.f, -10.f });

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_UI);
    renderer->SetMaterial("Minimap_Mtrl");

    return S_OK;
}

bool MinimapData::LoadFromMap(const std::wstring& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    file.read(reinterpret_cast<char*>(&width), sizeof(int));
    file.read(reinterpret_cast<char*>(&height), sizeof(int));

    mapData.resize(height, std::vector<TileInfo>(width));

    for (int z = 0; z < height; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            TileInfo tile;
            file.read(reinterpret_cast<char*>(&tile.walkable), sizeof(bool));

            tile.color = tile.walkable
                ? D3DCOLOR_ARGB(255, 255, 255, 255)
                : D3DCOLOR_ARGB(255, 80, 80, 80);

            mapData[z][x] = tile;
        }
    }

    file.close();
    return true;
}

bool MinimapData::GenerateFromChunks(ChunkManager* chunkMgr)
{
    auto& chunks = chunkMgr->GetChunks();
    if (chunks.empty()) return false;

    int minX = INT_MAX, maxX = INT_MIN;
    int minZ = INT_MAX, maxZ = INT_MIN;

    for (const auto& pair : chunks)
    {
        int cx = pair.first.first;
        int cz = pair.first.second;
        minX = min(minX, cx);
        maxX = max(maxX, cx);
        minZ = min(minZ, cz);
        maxZ = max(maxZ, cz);
    }

    width = (maxX - minX + 1) * CHUNK_SIZE;
    height = (maxZ - minZ + 1) * CHUNK_SIZE;

    mapData.resize(height, std::vector<TileInfo>(width));

    for (int worldZ = 0; worldZ < height; ++worldZ)
    {
        for (int worldX = 0; worldX < width; ++worldX)
        {
            int chunkX = minX + worldX / CHUNK_SIZE;
            int chunkZ = minZ + worldZ / CHUNK_SIZE;
            int localX = worldX % CHUNK_SIZE;
            int localZ = worldZ % CHUNK_SIZE;

            Chunk* chunk = chunkMgr->GetChunk(chunkX, chunkZ);
            if (!chunk)
            {
                mapData[worldZ][worldX] = { false, D3DCOLOR_ARGB(255, 0, 0, 0) };
                continue;
            }

            bool walkable = false;
            for (int y = CHUNK_HEIGHT - 2; y >= 1; --y)
            {
                auto block = chunk->GetBlock(localX, y, localZ);
                auto above = chunk->GetBlock(localX, y + 1, localZ);

                if (block.Type != Air && above.Type == Air)
                {
                    walkable = true;
                    break;
                }
            }

            mapData[worldZ][worldX].walkable = walkable;
            mapData[worldZ][worldX].color = walkable
                ? D3DCOLOR_ARGB(255, 255, 255, 255)
                : D3DCOLOR_ARGB(255, 80, 80, 80);
        }
    }

    mesh = MinimapMesh::Create();
    if (!mesh || FAILED(mesh->Ready_Mesh())) return false;

    auto renderer = GetComponent<MeshRenderer>();
    if (renderer) renderer->SetMesh(mesh);
    return true;
}

bool MinimapData::IsWalkable(int x, int z) const
{
    if (x < 0 || x >= width || z < 0 || z >= height) return false;
    return mapData[z][x].walkable;
}

_ulong MinimapData::GetColor(int x, int z) const
{
    if (x < 0 || x >= width || z < 0 || z >= height) return D3DCOLOR_ARGB(0, 0, 0, 0);
    return mapData[z][x].color;
}

void MinimapData::Free()
{
    Safe_Release(mesh);
}