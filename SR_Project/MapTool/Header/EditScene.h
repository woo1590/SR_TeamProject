#pragma once
#include "Scene.h"
#include "DynamicBlock.h"

class TerrainCreater;
class EditScene : public Scene
{
    friend class Scene;

private:
    explicit EditScene();
    virtual ~EditScene();

public:
    static EditScene* Create();
    void Load() override;
    void Update(float dt) override;
    void Late_Update(float dt) override;
    void Unload() override {};

private:

#ifdef USE_IMGUI
    void ImGui_Main();
    void ImGui_SaveLoad();
    void ImGui_SetBlockType();
    void ImGui_SetBlockUsage();
    void ImGui_SetBlockInfo();
    void ImGui_LinkLever();
    void ImGui_RemoveBlock();
#endif

    void MakePickingRay(_vec3& outRayOrigin, _vec3& outRayDir);     // 광선 위치와 방향 초기화
    bool RayIntersectsAABB(const _vec3& rayOrigin, const _vec3& rayDir, const _vec3& boxMin, const _vec3& boxMax, float& outDistance); // 광선 박스 충돌
    _vec3 GetHitNormal(const _vec3& hitPoint, const _vec3& boxMin, const _vec3& boxMax); // 충돌한 면의 법선 반환
    void Place(_vec3& position); // 블럭 설치

    void OnLeftClick(_vec3& rayOrigin, _vec3& rayDir);              // 좌클릭
    void OnRightClick(_vec3& rayOrigin, _vec3& rayDir);             // 우클릭

    void CreateTerrain(const std::string& filename);
    void PlaceTerrainBlocks(const std::string& filename);

    std::pair<int, int> GetChunkCoordFromWorldPos(const _vec3& pos)
    {
        int chunkX = static_cast<int>(round(pos.x / (CHUNK_SIZE * 2.f)));
        int chunkZ = static_cast<int>(round(pos.z / (CHUNK_SIZE * 2.f)));
        return { chunkX, chunkZ };
    }

    _vec3 GetLocalCoordInChunk(const _vec3& pos, int chunkX, int chunkZ)
    {
        float localX = (pos.x / 2.f) - chunkX * CHUNK_SIZE;
        float localY = (pos.y / 2.f);
        float localZ = (pos.z / 2.f) - chunkZ * CHUNK_SIZE;
        return { localX, localY, localZ };
    }

private:
    void Free() override;

private:
    int WidthX = 0;
    int WidthZ = 0;
    int Height = 0;
    float Scale = 0.f;

    TerrainCreater* Terrain = nullptr;

    int selectedSBlockUsage = 0;
    int selectedSBlockAxis = 1;
    int selectedSBlockRot = 0;
    int selectedSBlockType = static_cast<int>(StaticBlockType::Air);

    int selectedDBlockAxis = 0;
    int selectedDBlockRot = 0;
    int selectedDBlockType = static_cast<int>(DynamicBlockType::dBlockNone);

    StaticBlockUsage staticBlockUsage = StaticBlockUsage::Basic;
    StaticBlockType staticBlockType = StaticBlockType::Air;
    StaticBlockAxis staticBlockAxis = StaticBlockAxis::sAY;
    StaticBlockRot staticBlockRot = StaticBlockRot::sREnd;

    DynamicBlockCol dynamicBlockCol = DynamicBlockCol::dAEnd;
    DynamicBlockRot dynamicBlockRot = DynamicBlockRot::dREnd;
    DynamicBlockType dynamicBlockType = DynamicBlockType::dBlockNone;
};