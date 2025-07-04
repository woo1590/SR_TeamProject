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
    HRESULT SetMaterial(D3DMATERIAL9& mtrl);
    HRESULT SetTexture(const std::wstring& key);
    void Apply();

    /*---------Shader-----------*/
    void SetInt(const std::wstring& name, int value);
    void SetFloat(const std::wstring& name, float value);
    void SetVec3(const std::wstring& name, _vec3 value);
    void SetMat(const std::wstring& name, _matrix value);

private:
    void Free()override;

    D3DMATERIAL9 Mtrl{};
    LPDIRECT3DBASETEXTURE9 Texture = nullptr;

    /*---------------Shader---------------*/
    Shader* shader = nullptr;
    std::unordered_map<std::wstring, int> IntParam;
    std::unordered_map<std::wstring, float> FloatParam;
    std::unordered_map<std::wstring, _vec3> Vec3Param;
    std::unordered_map<std::wstring, _matrix> MatParam;
    /*------------------------------------*/

    LPDIRECT3DDEVICE9 Device = nullptr;
};

END