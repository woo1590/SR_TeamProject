#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class LightComponent;
class ENGINE_DLL LightSystem :
    public Base
{
private:
    explicit LightSystem();
    virtual ~LightSystem();

public:
    static LightSystem* Create();
    
    HRESULT Ready_LightSystem();
    void RegisterLight(LightComponent* light);
    void ApplyLight();

private:
    void Free()override;

    std::vector<LightComponent*> Lights;
};

END