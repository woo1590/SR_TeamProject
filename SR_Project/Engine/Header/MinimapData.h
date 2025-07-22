#pragma once
#include "Object.h"
#include "Engine_Struct.h"

BEGIN(Engine)
class Chunk;
class ChunkManager;
class MinimapMesh;
class ObjectManager;
class ENGINE_DLL MinimapData : public Object
{
private:
    explicit MinimapData(ObjectManager* owner);
    virtual ~MinimapData();

public:
    static MinimapData* Create(ObjectManager* owner);

    HRESULT Ready_Object() override;

    bool LoadFromMap(const std::wstring& path);
    bool GenerateFromChunks(ChunkManager* chunkMgr);

    bool IsWalkable(int x, int z) const;
    _ulong GetColor(int x, int z) const;

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

private:
    void Free() override;

private:
    int width = 0;
    int height = 0;

    MinimapMesh* mesh = nullptr;
    std::vector<std::vector<TileInfo>> mapData;
};
END