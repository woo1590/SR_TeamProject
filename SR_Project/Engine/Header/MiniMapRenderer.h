#pragma once
#include "RendererComponent.h"

BEGIN(Engine)
class ChunkManager;
class ObjectManager;
class ENGINE_DLL MiniMapRenderer : public RendererComponent
{
private:
    struct MiniMapTile
    {
        D3DXVECTOR2 screenPos;
        D3DCOLOR color;
    };

private:
    MiniMapRenderer(Object* owner, RENDER_ID id);

public:
    virtual ~MiniMapRenderer();
    static MiniMapRenderer* Create(Object* owner, RENDER_ID id);

    HRESULT Ready_Component() override;
    void Update(_float dt) override;
    void Render() override;

    void AddTile(const D3DXVECTOR2& pos, D3DCOLOR color);
    void ClearTiles();

    void SetVisible(bool visible) { IsVisible = visible; }
    bool GetVisible() { return IsVisible; }

    _vec2 WorldtoMapPos(const _vec3& worldPos, const _vec3& playerPos);
    void UpdateMapData(const _vec3& playerPos, ChunkManager* chunkMgr, SceneID sceneID);
    void UpdateMapData(ObjectManager* objectMgr, ChunkManager* chunkMgr, SceneID sceneID);

private:
    void Free() override;

private:
    bool IsVisible = false;
    int tileSize = 10;
    float Scale = 1.f;

    _vec2 MapCenter;
    _vec2 MapOffset{ 125.f, 125.f };

    ID3DXSprite* Sprite = nullptr;
    LPDIRECT3DTEXTURE9 TileTexture = nullptr;
    std::vector<MiniMapTile> Tiles;
};
END