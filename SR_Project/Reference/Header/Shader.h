#pragma once

BEGIN(Engine)

class ENGINE_DLL Shader : public Base
{
private:
    Shader();
    virtual ~Shader() {}

public:
    static Shader* Create();
    HRESULT Ready_Shader() { return S_OK; }

    void Apply();
    void SetConstant(const std::wstring& name, int value);
    void SetConstant(const std::wstring& name, float value);
    void SetConstant(const std::wstring& name, _vec3 value);
    void SetConstant(const std::wstring& name, _matrix value);

private:
    void Free()override {}

    LPDIRECT3DVERTEXSHADER9 VS = nullptr;
    LPDIRECT3DPIXELSHADER9 PS = nullptr;
    LPDIRECT3DDEVICE9 Device = nullptr;
};

END