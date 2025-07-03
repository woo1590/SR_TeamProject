#pragma once

BEGIN(Engine)

class ObjectManager;
class EventSystem;
class CollisionSystem;
class PhysicsSystem;
class CameraManager;
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
    EventSystem*        GetEventSystem()const;
    CollisionSystem*    GetCollisionSystem()const;
    PhysicsSystem*      GetPhysicsStstem()const;
    CameraManager*      GetCameraManager()const;
protected:
    void Free()override;

    ObjectManager* ObjectMgr = nullptr;
    EventSystem* EventSys = nullptr;
    CollisionSystem* CollisionSys = nullptr;
    PhysicsSystem* PhysicsSys = nullptr;
    CameraManager* CameraMgr = nullptr; 
};

END
