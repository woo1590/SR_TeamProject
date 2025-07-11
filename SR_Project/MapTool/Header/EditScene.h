#pragma once
#include "Scene.h"
#include "DynamicBlock.h"

namespace Engine
{
    class Chunk;
}

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
    void ImGui_Info();
    void ImGui_SaveLoad();
    void ImGui_SetBlockUsage();
    void ImGui_SetBlockType();
    void ImGui_SetBlockInfo();
    void ImGui_LinkLever();
#endif
    void CreateTerrain(const std::string& filename);
    void PlaceTerrainBlocks(const std::string& filename);

    // 광선 위치와 방향 초기화
    void MakePickingRay(_vec3& outRayOrigin, _vec3& outRayDir);

    // 광선 박스 충돌
    bool RayIntersectsAABB(const _vec3& rayOrigin, const _vec3& rayDir, const _vec3& boxMin, const _vec3& boxMax, float& outDistance);

    // 충돌한 면의 법선 반환
    _vec3 GetHitNormal(const _vec3& hitPoint, const _vec3& boxMin, const _vec3& boxMax);

    // 블럭 설치
    void Place(_vec3& position);

    void OnLeftClick(_vec3& rayOrigin, _vec3& rayDir);  // 좌클릭
    void OnRightClick(_vec3& rayOrigin, _vec3& rayDir); // 우클릭

    // 월드 좌표가 위치한 청크 위치 반환
    std::pair<int, int> GetChunkCoordFromWorldPos(const _vec3& pos);
   
    // 월드 좌표를 청크 내 로컬 좌표로 반환
    _vec3 GetLocalCoordInChunk(const _vec3& pos, int chunkX, int chunkZ);

private:
    void Free() override;

private:
    bool isDown = false;
    
    // ================ 지형 생성 ================
    bool CreateTer = false;             // 지형 생성 여부
    int WidthX = 0;                     // 지형 생성할 때, X 길이
    int WidthZ = 0;                     // 지형 생성할 때, Z 길이
    int Height = 0;                     // 지형 생성할 때, Y 높이
    float Scale = 0.f;                  // 지형 굴곡의 정도
    TerrainCreater* Terrain = nullptr;  // 지형 생성 함수 사용을 위한 변수

    // ================ Static Block ================
    // 블럭 모양 결정
    int selectedSBlockUsage = 0;
    StaticBlockUsage staticBlockUsage = StaticBlockUsage::Basic;

    // 블럭 종류 결정
    int selectedSBlockType = static_cast<int>(StaticBlockType::Air);
    StaticBlockType staticBlockType = StaticBlockType::Air;

    // 블럭 Y 방향이 가리키는 방향 결정
    int selectedSBlockAxis = 1;
    StaticBlockAxis staticBlockAxis = StaticBlockAxis::sAY;

    // 블럭 Y축 회전 방향 결정
    int selectedSBlockRot = 0;
    StaticBlockRot staticBlockRot = StaticBlockRot::sREnd;

    // ================ Dynamic Block ================
    // 설치당하는 블럭의 면의 방향
    DynamicBlockCol dynamicBlockCol = DynamicBlockCol::dAEnd;

    // 블럭 종류 결정
    int selectedDBlockType = static_cast<int>(DynamicBlockType::dBlockNone);
    DynamicBlockType dynamicBlockType = DynamicBlockType::dBlockNone;

    // 블럭 Y축 회전 방향 결정
    int selectedDBlockRot = 0;
    DynamicBlockRot dynamicBlockRot = DynamicBlockRot::dREnd;
};