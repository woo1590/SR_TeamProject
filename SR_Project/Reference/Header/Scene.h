#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ObjectManager;
class Object;
class ENGINE_DLL Scene :
    public Base
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

protected:
    void Free()override;

    ObjectManager* ObjectMgr;
    Object* Camera;
};

END
