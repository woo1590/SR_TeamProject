#pragma once
#include "Base.h"

BEGIN(Engine)
class Scene;
class MinimapData;
class MinimapMesh;
class ChunkManager;
class ENGINE_DLL MinimapManager : public Base
{
private:
    MinimapManager(Scene* owner);
    virtual ~MinimapManager();

public:
    static MinimapManager* Create(Scene* owner);

    bool LoadMiniMap(const std::wstring& filepath);
    bool GenerateFromChunks(ChunkManager* chunkManager);
    void ClearMiniMap();

    MinimapMesh* GetMiniMapMesh() { return MinimapMesh; }

private:
    void Free() override;

private:
    Scene* owner = nullptr;
    MinimapData* MinimapData = nullptr;
    MinimapMesh* MinimapMesh = nullptr;
};
END