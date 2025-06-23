#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class ENGINE_DLL ReflectComponent :
    public ObjectComponent
{
private:
    ReflectComponent(Object* owner);
    virtual ~ReflectComponent();

public:
    static ReflectComponent* Create(Object* owner);

private:
    D3DXPLANE ReflectPlane;
};

END