#pragma once

BEGIN(Engine)

class ENGINE_DLL Shader : public Base
{
private:
    Shader(LPD3DXEFFECT effect );
    virtual ~Shader() {}

public:
    static Shader* Create(LPD3DXEFFECT effect);
    HRESULT Ready_Shader();

    HRESULT Begin(UINT passIndex);
    HRESULT End();

    void SetConstant(const std::string& name, int value);
    void SetConstant(const std::string& name, float value);
    void SetConstant(const std::string& name, const _vec3& value);
    void SetConstant(const std::string& name, const _matrix& value);
    void SetTexture(const std::string& name, LPDIRECT3DBASETEXTURE9 tex);

    Shader* Clone();
private:
    void Free()override {}

    LPD3DXEFFECT effect = nullptr;
    LPDIRECT3DDEVICE9 Device = nullptr;
};

END