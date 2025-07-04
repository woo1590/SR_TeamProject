#include "EnginePCH.h"
#include "ResourceManager.h"
#include "GraphicDevice.h"
#include "Mesh.h"
#include "Material.h"
#include "TerrainMesh.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "GraphicDevice.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourceManager* ResourceManager::Create()
{
    ResourceManager* Instance = new ResourceManager;

    if (FAILED(Instance->Ready_ResourceManager()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT ResourceManager::Ready_ResourceManager()
{
    return S_OK;
}

void ResourceManager::LoadResource(const std::wstring& texPath, const std::wstring& texKey, TEXTURE texType, const std::wstring& mtrlKey)
{
    LoadTexture(texPath, texKey, texType);
    auto mtrl = Material::Create();
    mtrl->SetTexture(texKey);
    LoadMaterial(mtrlKey, mtrl);
}

void ResourceManager::LoadVertexShader(const std::wstring& vsPath, const std::string& vsEntry, const std::wstring& key)
{
    auto device = GraphicDevice::GetInstance()->GetDevice();

    LPD3DXBUFFER code = nullptr;
    LPD3DXBUFFER error = nullptr;

    D3DXCompileShaderFromFileW(vsPath.c_str(), nullptr, nullptr, vsEntry.c_str(), "vs_3_0", 0, &code, &error, nullptr);

    LPDIRECT3DVERTEXSHADER9 vs = nullptr;
    device->CreateVertexShader((DWORD*)code->GetBufferPointer(), &vs);
    code->Release();
    error->Release();

    auto shader = Shader::Create(vs, nullptr);
    ShaderContainer[key] = shader;
}

void ResourceManager::LoadPixelShader(const std::wstring& psPath, const std::string& psEntry, const std::wstring& key)
{
    auto device = GraphicDevice::GetInstance()->GetDevice();

    LPD3DXBUFFER code = nullptr;
    LPD3DXBUFFER error = nullptr;

    D3DXCompileShaderFromFileW(psPath.c_str(), nullptr, nullptr, psEntry.c_str(), "ps_3_0", 0, &code, &error, nullptr);

    LPDIRECT3DPIXELSHADER9 ps = nullptr;
    device->CreatePixelShader((DWORD*)code->GetBufferPointer(), &ps);
    code->Release();
    error->Release();

    auto shader = Shader::Create(nullptr, ps);
    ShaderContainer[key] = shader;
}

void ResourceManager::LoadShader(const std::wstring& vsPath, const std::string& vsEntry, const std::wstring& psPath, const std::string& psEntry, const std::wstring& key)
{
    auto device = GraphicDevice::GetInstance()->GetDevice();

    LPD3DXBUFFER code = nullptr;
    LPD3DXBUFFER error = nullptr;

    D3DXCompileShaderFromFileW(vsPath.c_str(), nullptr, nullptr, vsEntry.c_str(), "vs_3_0", 0, &code, &error, nullptr);

    LPDIRECT3DVERTEXSHADER9 vs = nullptr;
    device->CreateVertexShader((DWORD*)code->GetBufferPointer(), &vs);
    code->Release();
    error->Release();

    D3DXCompileShaderFromFileW(psPath.c_str(), nullptr, nullptr, psEntry.c_str(), "ps_3_0", 0, &code, &error, nullptr);

    LPDIRECT3DPIXELSHADER9 ps = nullptr;
    device->CreatePixelShader((DWORD*)code->GetBufferPointer(), &ps);
    code->Release();
    error->Release();

    auto shader = Shader::Create(vs, ps);
    ShaderContainer[key] = shader;  
}

void ResourceManager::LoadTexture(const std::wstring& filePath, const std::wstring& key, TEXTURE texType)
{
    LPDIRECT3DBASETEXTURE9 tex = nullptr;
    auto device = GraphicDevice::GetInstance()->GetDevice();

    switch (texType)
    {
    case Engine::TEXTURE::Tex_Diffuse:
        D3DXCreateTextureFromFileW(device, filePath.c_str(), (LPDIRECT3DTEXTURE9*)&tex);
        break;
    case Engine::TEXTURE::Tex_Cube:
        D3DXCreateCubeTextureFromFileW(device, filePath.c_str(), (LPDIRECT3DCUBETEXTURE9*)&tex);
        break;
    default:
        break;
    }

    TextureContainer[key] = tex;
}

void ResourceManager::LoadMesh(const std::wstring& key, Mesh* mesh)
{
    MeshContainer[key] = mesh;
}

void ResourceManager::LoadMaterial(const std::wstring& key, Material* mtrl)
{
    MaterialContainer[key] = mtrl;
}

Mesh* ResourceManager::GetMesh(const std::wstring& key)
{
    auto iter = MeshContainer.find(key);

    if (iter != MeshContainer.end())
        return iter->second;
    else
        return nullptr;
}

Material* ResourceManager::GetMaterial(const std::wstring& key)
{
    auto iter = MaterialContainer.find(key);

    if (iter != MaterialContainer.end())
        return iter->second;
    else
        return nullptr;
}

LPDIRECT3DBASETEXTURE9 ResourceManager::GetTexture(const std::wstring& key)
{
    auto iter = TextureContainer.find(key);

    if (iter != TextureContainer.end())
        return iter->second;
    else
        return nullptr;
}

void ResourceManager::RegisterUILayer(const wstring& tag, int layer)
{
    uiLayerTable.emplace(tag, layer);
}

optional<int> ResourceManager::GetUILayer(const wstring& tag) const
{
    auto it = uiLayerTable.find(tag);
    return (it == uiLayerTable.end()) ? nullopt : optional<int>(it->second);
}

void ResourceManager::Free()
{
    std::for_each(MeshContainer.begin(), MeshContainer.end(), [](auto& pair)
        {
            Safe_Release(pair.second);
        });

    std::for_each(MaterialContainer.begin(), MaterialContainer.end(), [](auto& pair)
        {
            Safe_Release(pair.second);
        });

    std::for_each(TextureContainer.begin(), TextureContainer.end(), [](auto& pair)
        {
            Safe_Release(pair.second);
        });

}
