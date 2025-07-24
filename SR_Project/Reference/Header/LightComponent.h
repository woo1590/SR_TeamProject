#pragma once
#include "ObjectComponent.h"

enum class LightType { Point, Directional, Spot, Count };

BEGIN(Engine)

struct LightInfo
{
    _float range = 0.f;
    _vec3 position;
    _vec3 color;
};

class ENGINE_DLL LightComponent : public ObjectComponent
{
private:
    explicit LightComponent(Object* owner);
    virtual ~LightComponent();

public:
    static LightComponent* Create(Object* owner);
    void Late_Update(_float dt)override;

    void SetLightInfo(float range, _vec3 color);
    LightInfo GetLightInfo()const { return lightInfo; }
    bool IsEnabled()const;
    void SetEnabled(bool enable);

private:
    void Free()override;

    LightInfo lightInfo{};
    _bool isEnable = false;
};

END