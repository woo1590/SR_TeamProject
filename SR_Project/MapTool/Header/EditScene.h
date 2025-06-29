#pragma once
#include "Scene.h"

namespace Engine
{
    class TransformComponent;
}

class EditScene : public Scene
{
    friend class Scene;
    
private:
    enum BlockType { Dirt };

    struct BlockData
    {
        _vec3 Pos;
        BlockType Type;
    };

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
    void MakePickingRay(_vec3& outRayOrigin, _vec3& outRayDir);
    void OnLeftClick(_vec3& rayOrigin, _vec3& rayDir);
    
    bool RayIntersectsAABB(const _vec3& rayOrigin, const _vec3& rayDir, const _vec3& boxMin, const _vec3& boxMax, float& outDistance);
    _vec3 GetHitNormal(const _vec3& hitPoint, const _vec3& boxMin, const _vec3& boxMax);
    void PlaceBlock(const _vec3& position, BlockType type);

private:
    void Free() override;

private:
    std::vector<BlockData> Blocks;
};