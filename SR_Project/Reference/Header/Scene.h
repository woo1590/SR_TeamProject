#pragma once

BEGIN(Engine)
class UIManager;
class ObjectManager;
class CollisionSystem;
class PhysicsSystem;
class CameraManager;
class StaticGrid;
class BlockManager;
class ChunkManager;
class MinimapManager;
class Object;

class ENGINE_DLL Scene : public Base
{
protected:
    explicit Scene();
    virtual ~Scene();

public:
    template<typename T>
    static Scene* Create()
    {
        Scene* Instance = new T;
        if (FAILED(Instance->Ready_Scene()))
        {
            Safe_Release(Instance);

            Instance = nullptr;
        }

        return Instance;
    }

    HRESULT Ready_Scene();

    virtual void Load()PURE;
    virtual void Update(float dt)PURE;
    virtual void Late_Update(float dt)PURE;
    virtual void Unload()PURE;

    ObjectManager*      GetObjectManager()const;
    CollisionSystem*    GetCollisionSystem()const;
    PhysicsSystem*      GetPhysicsStstem()const;
    CameraManager*      GetCameraManager()const;
    StaticGrid*         GetStaticGrid()const;
    BlockManager*       GetBlockManager()const;
    ChunkManager*       GetChunkManager()const;
    UIManager*          GetUIManager()const;
    MinimapManager*     GetMinimapManager()const;
    
public:
    vector<SB>& GetStaticBlocks() { return staticBlocks; }
    vector<DB>& GetDynamicBlocks() { return dynamicBlocks; }

protected:
    int Count = 0;
    std::vector<SB> staticBlocks;
    std::vector<DB> dynamicBlocks;

protected:
    void Free()override;

    StaticGrid* Grid = nullptr;
    ObjectManager* ObjectMgr = nullptr;
    CollisionSystem* CollisionSys = nullptr;
    PhysicsSystem* PhysicsSys = nullptr;
    CameraManager* CameraMgr = nullptr; 
    BlockManager* BlockMgr = nullptr;
    ChunkManager* ChunkMgr = nullptr;
    UIManager* uiMgr = nullptr;
    MinimapManager* MinimapMgr = nullptr;
};

END
