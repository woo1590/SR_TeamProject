#include "EnginePCH.h"
#include "Material.h"
#include "GraphicDevice.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "Shader.h"

Material::Material()
    :Device(GraphicDevice::GetInstance()->GetDevice())
{
    Device->AddRef();
}

Material::~Material()
{
}

Material* Material::Create()
{
    Material* Instance = new Material;
    if (FAILED(Instance->Ready_Material()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Material::Ready_Material()
{   
    //Default Material
    Mtrl.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
    Mtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    Mtrl.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
    Mtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
    Mtrl.Power = 1.f;

    return S_OK;
}

HRESULT Material::SetMaterial(D3DMATERIAL9& mtrl)
{
    Mtrl = mtrl;

    return S_OK;
}

HRESULT Material::SetTexture(const std::wstring& key)
{
    auto tex = EngineCore::GetInstance()->GetResourceManager()->GetTexture(key);
    if (!tex)
        return E_FAIL;

    Texture = tex;

    return S_OK;
}

void Material::Apply()
{
    if (shader)
    {
        for (const auto& [name, value] : IntParam)
            shader->SetConstant(name, value);

        for (const auto& [name, value] : FloatParam)
            shader->SetConstant(name, value);

        for (const auto& [name, value] : Vec3Param)
            shader->SetConstant(name, value);

        for (const auto& [name, value] : MatParam)
            shader->SetConstant(name, value);

        for (const auto& [name, value] : TexParam)
            shader->SetTexture(name, value);
    }
    else
    {
        Device->SetMaterial(&Mtrl);
        Device->SetTexture(0, Texture);
    }
}

void Material::SetInt(const std::string& name, int value)
{
    IntParam[name] = value;
}

void Material::SetFloat(const std::string& name, float value)
{
    FloatParam[name] = value;
}

void Material::SetVec3(const std::string& name, _vec3 value)
{
    Vec3Param[name] = value;
}

void Material::SetMat(const std::string& name, _matrix value)
{
    MatParam[name] = value;
}

void Material::SetTexture(const std::string& name, LPDIRECT3DBASETEXTURE9 value)
{
    TexParam[name] = value;
}

void Material::SetShader(const std::wstring& key)
{
    shader = EngineCore::GetInstance()->GetResourceManager()->GetShader(key);
}

Shader* Material::GetShader() const
{
    return shader;
}

void Material::Free()
{
    Safe_Release(Texture);
    Safe_Release(Device);
}
