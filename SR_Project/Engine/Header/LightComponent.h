#pragma once
#include "ObjectComponent.h"

enum class LightType { Point, Directional, Spot, Count };

BEGIN(Engine)

class ENGINE_DLL LightComponent : public ObjectComponent
{
private:
    explicit LightComponent(Object* owner);
    virtual ~LightComponent();

public:
    static LightComponent* Create(Object* owner);

    void SetPointLight(float range, D3DCOLOR color);
    void SetDirectionalLight(_vec3 direction, D3DCOLOR color);

    const D3DLIGHT9& GetLightData()const;
    bool IsEnabled()const;
    void SetEnabled(bool enable);

private:
    void Free()override;

    D3DLIGHT9 LightData{};
    LightType Type;
    bool Is_Enabled = true;
};

END