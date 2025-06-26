#include "EnginePCH.h"
#include "Material.h"
#include "GraphicDevice.h"

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
    Mtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
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

HRESULT Material::SetTexture(const std::wstring& filePath, TEXTURE texType)
{
    switch (texType)
    {
    case Engine::TEXTURE::Tex_Normal:
        D3DXCreateTextureFromFileW(Device, filePath.c_str(), (LPDIRECT3DTEXTURE9*)&Texture);
        break;
    case Engine::TEXTURE::Tex_Cube:
        D3DXCreateCubeTextureFromFileW(Device, filePath.c_str(), (LPDIRECT3DCUBETEXTURE9*)&Texture);
        break;
    default:
        break;
    }

    return S_OK;
}

void Material::Apply()
{
    Device->SetMaterial(&Mtrl);
    Device->SetTexture(0, Texture);
}

void Material::Free()
{
    Safe_Release(Texture);
    Safe_Release(Device);
}
