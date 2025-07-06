#pragma once

BEGIN(Engine)

class ENGINE_DLL Shader : public Base
{
private:
    Shader();
    virtual ~Shader() {}

public:
    static Shader* Create(const void * vsCode, const void* psCode);
    HRESULT Ready_Shader(const void* vsCode, const void* psCode);

    void Apply();
    void SetConstant(const std::string& name, int value);
    void SetConstant(const std::string& name, float value);
    void SetConstant(const std::string& name, _vec3 value);
    void SetConstant(const std::string& name, _matrix value);

private:
    void Free()override {}

    LPDIRECT3DVERTEXSHADER9 VS = nullptr;
    LPDIRECT3DPIXELSHADER9 PS = nullptr;
    LPD3DXCONSTANTTABLE vsConst = nullptr;
    LPD3DXCONSTANTTABLE psConst = nullptr;

    LPDIRECT3DDEVICE9 Device = nullptr;
};

END