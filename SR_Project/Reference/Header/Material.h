#pragma once

BEGIN(Engine)

class Shader;
class ENGINE_DLL Material : public Base
{
private:
    Material();
    virtual ~Material();

public:
    static Material* Create();
    HRESULT Ready_Material();
    void Apply();

    /*---------Shader-----------*/
    void SetInt(const std::string & name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec3(const std::string& name, _vec3 value);
    void SetVec4(const std::string& name, _vec4 value);
    void SetMat(const std::string& name, _matrix value);
    void SetTexture(const std::string& name, LPDIRECT3DBASETEXTURE9 value);
    void SetShader(Shader* shader);
    Shader* GetShader()const;

    Material* Clone();
private:
    void Free()override;

    /*---------------Shader---------------*/
    Shader* shader = nullptr;
    std::unordered_map<std::string, int> IntParam;
    std::unordered_map<std::string, float> FloatParam;
    std::unordered_map<std::string, _vec3> Vec3Param;
    std::unordered_map<std::string, _vec4> Vec4Param;
    std::unordered_map<std::string, _matrix> MatParam;
    std::unordered_map<std::string, LPDIRECT3DBASETEXTURE9> TexParam;

    LPDIRECT3DDEVICE9 Device = nullptr;
    /*------------------------------------*/
};

END