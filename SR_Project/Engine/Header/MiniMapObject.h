#pragma once
#include "Object.h"

BEGIN(Engine)
class ObjectManager;
class MiniMapRenderer;
class ENGINE_DLL MiniMapObject : public Object
{
private:
    MiniMapObject(ObjectManager* owner);
    virtual ~MiniMapObject();

public:
    static MiniMapObject* Create(ObjectManager* owner);

public:
    HRESULT Ready_Object() override;
    MiniMapRenderer* GetMiniMapRenderer() const { return miniMapRenderer; }

private:
    MiniMapRenderer* miniMapRenderer = nullptr;
};
END