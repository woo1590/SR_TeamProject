#include "EnginePCH.h"
#include "MiniMapRenderer.h"
#include "EngineCore.h"
#include "GraphicDevice.h"
#include "ChunkManager.h"
#include "ObjectManager.h"

#include "TransformComponent.h"

#include "Object.h"
#include "DynamicBlock.h"

MiniMapRenderer::MiniMapRenderer(Object* owner, RENDER_ID id) : RendererComponent(owner, id)
{
}

MiniMapRenderer::~MiniMapRenderer()
{
}

MiniMapRenderer* MiniMapRenderer::Create(Object* owner, RENDER_ID id)
{
    MiniMapRenderer* instance = new MiniMapRenderer(owner, id);
    if (FAILED(instance->Ready_Component())) Safe_Release(instance);
    return instance;
}

HRESULT MiniMapRenderer::Ready_Component()
{
    LPDIRECT3DDEVICE9 device = GraphicDevice::GetInstance()->GetDevice();
    if (FAILED(D3DXCreateTexture(device, 1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &TileTexture))) return E_FAIL;

    D3DLOCKED_RECT rect;
    TileTexture->LockRect(0, &rect, nullptr, 0);
    *((DWORD*)rect.pBits) = 0xFFFFFFFF;
    TileTexture->UnlockRect(0);

    if (FAILED(D3DXCreateSprite(device, &Sprite))) return E_FAIL;
    return S_OK;
}

void MiniMapRenderer::Update(_float dt)
{
}

void MiniMapRenderer::Render()
{
    if (!IsVisible) return;
    if (!Sprite || !TileTexture) return;

    Sprite->Begin(D3DXSPRITE_ALPHABLEND);

    for (const auto& tile : Tiles)
    {
        RECT rect = { 0, 0, tileSize, tileSize };
        _vec3 pos(tile.screenPos.x, tile.screenPos.y, 0);
        Sprite->Draw(TileTexture, &rect, nullptr, &pos, tile.color);
    }

    Sprite->End();
}

void MiniMapRenderer::AddTile(const D3DXVECTOR2& pos, D3DCOLOR color)
{
    Tiles.push_back({ pos + MapOffset, color});
}

void MiniMapRenderer::ClearTiles()
{
    Tiles.clear();
}

_vec2 MiniMapRenderer::WorldtoMapPos(const _vec3& worldPos, const _vec3& playerPos)
{
    _vec2 offset(worldPos.x - playerPos.x, worldPos.z - playerPos.z);
    offset *= Scale;
    return MapCenter + offset;
}

void MiniMapRenderer::UpdateMapData(const _vec3& playerPos, ChunkManager* chunkMgr, SceneID sceneID)
{
    ClearTiles();

    int range(8);
    int playerChunkX(static_cast<int>(playerPos.x) / CHUNK_SIZE);
    int playerChunkZ(static_cast<int>(playerPos.z) / CHUNK_SIZE);

    for (int dx = -range; dx <= range; ++dx)
    {
        for (int dz = -range; dz <= range; ++dz)
        {
            MINIMAP miniMapData;
            if (chunkMgr->GetMiniMapChunk(playerChunkX + dx, playerChunkZ + dz, miniMapData))
            {
                for (int x = 0; x < CHUNK_SIZE; ++x)
                {
                    for (int z = 0; z < CHUNK_SIZE; ++z)
                    {
                        if (miniMapData.path[x][z])
                        {
                            float worldX = (playerChunkX + dx) * CHUNK_SIZE + x;
                            float worldZ = (playerChunkZ + dz) * CHUNK_SIZE + z;

                            D3DXVECTOR2 miniMapPos = WorldtoMapPos(D3DXVECTOR3(worldX, 0, worldZ), playerPos);

                            switch (sceneID)
                            {
                            case TUTORIAL:
                                AddTile(miniMapPos, D3DCOLOR_ARGB(255, 60, 79, 37));
                                break;
                            case STAGE1:
                                AddTile(miniMapPos, D3DCOLOR_ARGB(255, 117, 180, 75));
                                break;
                            case STAGE2:
                                AddTile(miniMapPos, D3DCOLOR_ARGB(255, 255, 255, 255));
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    AddTile(MapCenter, D3DCOLOR_ARGB(255, 255, 0, 0));
}

void MiniMapRenderer::UpdateMapData(ObjectManager* objectMgr, ChunkManager* chunkMgr, SceneID sceneID)
{
    ClearTiles();
    auto playerTransform = objectMgr->GetFrontObject(ObjectType::Camera)->GetComponent<TransformComponent>();
    _vec3 playerPos = playerTransform->GetPosition();

    MapCenter = D3DXVECTOR2(0.f, 0.f);

    int range(4);
    int playerChunkX(static_cast<int>(playerPos.x) / CHUNK_SIZE);
    int playerChunkZ(static_cast<int>(playerPos.z) / CHUNK_SIZE);

    for (int dx = -range; dx <= range; ++dx)
    {
        for (int dz = -range; dz <= range; ++dz)
        {
            MINIMAP miniMapData;
            if (chunkMgr->GetMiniMapChunk(playerChunkX + dx, playerChunkZ + dz, miniMapData))
            {
                for (int x = 0; x < CHUNK_SIZE; ++x)
                {
                    for (int z = 0; z < CHUNK_SIZE; ++z)
                    {
                        if (miniMapData.path[x][z])
                        {
                            float worldX = (playerChunkX + dx) * CHUNK_SIZE + x;
                            float worldZ = (playerChunkZ + dz) * CHUNK_SIZE + z;
                            D3DXVECTOR2 miniMapPos = WorldtoMapPos(D3DXVECTOR3(worldX, 0, worldZ), playerPos);

                            switch (sceneID)
                            {
                            case TUTORIAL:
                                AddTile(miniMapPos, D3DCOLOR_ARGB(255, 60, 79, 37));
                                break;
                            case STAGE1:
                                AddTile(miniMapPos, D3DCOLOR_ARGB(255, 117, 180, 75));
                                break;
                            case STAGE2:
                                AddTile(miniMapPos, D3DCOLOR_ARGB(255, 255, 255, 255));
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    auto monsters = objectMgr->GetObjectList(ObjectType::Monster);
    for (auto& monster : monsters)
    {
        auto mobTrans = monster->GetComponent<TransformComponent>();
    
        _vec3 distance = mobTrans->GetPosition() - playerPos;
        if (D3DXVec3Length(&distance) > 100.f) continue;
    
        _vec2 miniMapPos = WorldtoMapPos(mobTrans->GetWorldPosition(), playerPos);
        AddTile(miniMapPos, D3DCOLOR_ARGB(255, 255, 255, 0));
    }

    auto dynamics = objectMgr->GetObjectList(ObjectType::DynamicBlock);
    for (auto& dynamic : dynamics)
    {
        DynamicBlock* dynBlock = static_cast<DynamicBlock*>(dynamic);
        auto trans = dynamic->GetComponent<TransformComponent>();

        if (dynBlock->GetType() == LeverSwitch)
        {
            _vec3 distance = trans->GetPosition() - playerPos;
            if (D3DXVec3Length(&distance) > 80.f) continue;

            _vec2 miniMapPos = WorldtoMapPos(trans->GetWorldPosition(), playerPos);
            AddTile(miniMapPos, D3DCOLOR_ARGB(255, 0, 128, 255));
        }
        else if (dynBlock->GetType() == Bridges && dynBlock->GetTrigger())
        {
            _vec3 bridgePos = trans->GetWorldPosition();

            for (int z = 0; z <= 150; z += 2)
            {
                float posX = bridgePos.x + 4.f * 2.f;
                float posZ = bridgePos.z - static_cast<float>(z);

                _vec2 miniMapPos = WorldtoMapPos({ posX, 0.f, posZ }, playerPos);
                AddTile(miniMapPos, D3DCOLOR_ARGB(255, 255, 255, 255));
            }
        }
    }

    AddTile(MapCenter, D3DCOLOR_ARGB(255, 255, 0, 0));
}

void MiniMapRenderer::Free()
{
    Safe_Release(TileTexture);
    Safe_Release(Sprite);
}