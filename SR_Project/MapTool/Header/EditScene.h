#pragma once
#include "Scene.h"
#include "DynamicBlock.h"

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

private:
    void Free() override;

private:
    int Width = 0;
    int Height = 0;
    float Scale = 0.f;

    int selectedSBlockUsage = 0;
    int selectedSBlockAxis = 1;
    int selectedSBlockRot = 0;
    int selectedSBlockType = static_cast<int>(StaticBlockType::sBlockNone);

    int selectedDBlockAxis = 0;
    int selectedDBlockRot = 0;
    int selectedDBlockType = static_cast<int>(DynamicBlockType::dBlockNone);

    StaticBlockUsage staticBlockUsage = StaticBlockUsage::Basic;
    StaticBlockType staticBlockType = StaticBlockType::sBlockNone;
    StaticBlockAxis staticBlockAxis = StaticBlockAxis::sAY;
    StaticBlockRot staticBlockRot = StaticBlockRot::sREnd;

    DynamicBlockCol dynamicBlockCol = DynamicBlockCol::dAEnd;
    DynamicBlockRot dynamicBlockRot = DynamicBlockRot::dREnd;
    DynamicBlockType dynamicBlockType = DynamicBlockType::dBlockNone;
};