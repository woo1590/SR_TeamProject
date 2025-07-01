#pragma once
#include "Base.h"

BEGIN(Engine)

class Scene;
class Object;
class CameraComponent;
class ENGINE_DLL CameraManager :
    public Base
{
private:
    CameraManager(Scene* owner);
    virtual ~CameraManager();

public:
    static CameraManager* Create(Scene* owner);
    HRESULT Ready_CameraManager();

    void AddCamera(const std::wstring& key, Object* camera);
    bool SetMainCamera(const std::wstring& key);
    CameraComponent* GetMainCamera();
private:
    std::unordered_map<std::wstring, Object*> CameraMap;
    Object* MainCamera = nullptr;

    Scene* owner;

    void Free()override;
};

END