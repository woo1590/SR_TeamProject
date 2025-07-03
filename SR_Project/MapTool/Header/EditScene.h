#pragma once
#include "Scene.h"

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
    void Unload() override;

private:

#ifdef USE_IMGUI
    void ImGuiTest(); // ImGUI 작성SetIsBlock
#endif

    void MakePickingRay(_vec3& outRayOrigin, _vec3& outRayDir);     // 광선 위치와 방향 초기화
    void OnLeftClick(_vec3& rayOrigin, _vec3& rayDir);              // 좌클릭
    void OnRightClick(_vec3& rayOrigin, _vec3& rayDir);             // 우클릭
    
    bool RayIntersectsAABB(const _vec3& rayOrigin, const _vec3& rayDir, const _vec3& boxMin, const _vec3& boxMax, float& outDistance); // 광선 박스 충돌
    _vec3 GetHitNormal(const _vec3& hitPoint, const _vec3& boxMin, const _vec3& boxMax); // 충돌한 면의 법선 반환
    void Place(const _vec3& position); // 블럭 설치

    void SaveStage(const char* saveStage);   // 맵 저장
    void LoadStage(const char* loadStage);   // 맵 로드

private:
    void Free() override;

private:
    int selectedSBlockDir = 1;
    int selectedSBlockType = static_cast<int>(StaticBlockType::SBlockNone);

    int selectedDBlockDir = 0;
    int selectedDBlockType = static_cast<int>(DynamicBlockType::DBlockNone);

    int Count = 0;

    StaticBlockDir staticBlockDir = StaticBlockDir::BlockY;
    StaticBlockType staticBlockType = StaticBlockType::SBlockNone;

    DynamicBlockDir dynamicBlockDir = DynamicBlockDir::DBEnd;
    DynamicBlockType dynamicBlockType = DynamicBlockType::DBlockNone;

    std::vector<SB> staticBlocks;
    std::vector<DB> dynamicBlocks;
};