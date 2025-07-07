#include "EnginePCH.h"
#include "EngineCore.h"
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

//void ResourceManager::LoadResource(const std::wstring& texPath, const std::wstring& texKey, TEXTURE texType, const std::wstring& mtrlKey)
//{
//    LoadTexture(texPath, texKey, texType);
//    auto mtrl = Material::Create();
//    mtrl->SetTexture(texKey);
//    LoadMaterial(mtrlKey, mtrl);
//}

//void ResourceManager::LoadShader(const std::wstring& filePath, const std::wstring& key)
//{
//    auto device = GraphicDevice::GetInstance()->GetDevice();
//
//    LPD3DXEFFECT effect = nullptr;
//    LPD3DXBUFFER error = nullptr;
//    D3DXCreateEffectFromFileW(device, filePath.c_str(), nullptr, nullptr, 0, nullptr, &effect, &error);
//
//    if (error) {
//        OutputDebugStringA((char*)error->GetBufferPointer());
//        error->Release();
//    }
//
//    auto shader = Shader::Create(effect);
//
//    ShaderContainer[key] = shader;
//}
//
//void ResourceManager::LoadTexture(const std::wstring& filePath, const std::wstring& key, TEXTURE texType)
//{
//    LPDIRECT3DBASETEXTURE9 tex = nullptr;
//    auto device = GraphicDevice::GetInstance()->GetDevice();
//
//    switch (texType)
//    {
//    case Engine::TEXTURE::Tex_Diffuse:
//        D3DXCreateTextureFromFileW(device, filePath.c_str(), (LPDIRECT3DTEXTURE9*)&tex);
//        break;
//    case Engine::TEXTURE::Tex_Cube:
//        D3DXCreateCubeTextureFromFileW(device, filePath.c_str(), (LPDIRECT3DCUBETEXTURE9*)&tex);
//        break;
//    default:
//        break;
//    }
//
//    TextureContainer[key] = tex;
//}
//

//void ResourceManager::LoadMaterial(const std::wstring& key, Material* mtrl)
//{
//    MaterialContainer[key] = mtrl;
//}

void ResourceManager::LoadMesh(const std::string & key, Mesh* mesh)
{
    MeshContainer[key] = mesh;
}

void ResourceManager::LoadMaterial(const std::string& key, const std::string& filePath)
{
    auto mtrl = Material::Create();

    nlohmann::json j = nlohmann::json::parse(std::ifstream(filePath.c_str()));

    //Shader
    auto shader = LoadShader(j.value("fx", "Shaders/default.fx"));
    mtrl->SetShader(shader);

    //Texture
    for (auto& [slot, p] : j["textures"].items())
    {
        auto tex = (slot == "CubeMap") ? LoadTexture(p.get<std::string>(), TEXTURE::Tex_Cube)
                                       : LoadTexture(p.get<std::string>(), TEXTURE::Tex_Diffuse);

        mtrl->SetTexture(slot, tex);
    }

    //Constant
    if (j.contains("constants"))
    {
        auto& c = j["constants"];

        auto GetVec4 = [](const nlohmann::json& arr)->_vec4 {return _vec4(arr[0], arr[1], arr[2], arr[4]);};

        if (c.contains("Diffuse"))
            mtrl->SetVec4("g_Diffuse", GetVec4(c["Diffuse"]));

        if (c.contains("Ambient"))
            mtrl->SetVec4("g_Ambient", _vec4(c["Ambient"][0], c["Ambient"][1], c["Ambient"][2], 1.f));
        
        if (c.contains("Specular"))
            mtrl->SetVec4("g_Specular", _vec4(c["Specular"][0], c["Specular"][1], c["Specular"][2], 1.f));
        mtrl->SetFloat("g_Shininess",c.value("Shininess", 0.f));

        if (c.contains("AlphaCut"))
            mtrl->SetFloat("g_AlphaCur", c.value("AlphaCut", 0.f));

        if (c.contains("UVScale"))
            mtrl->SetVec4("g_UVScale", _vec4(c["UVScale"][0], c["UVScale"][1], 0.f, 0.f));

        if (c.contains("UVOffset"))
            mtrl->SetVec4("g_UVOffset", _vec4(c["UVOffset"][0], c["UVOffset"][1], 0.f, 0.f));

        if (c.contains("Emissive"))
            mtrl->SetVec4("g_Emissive", _vec4(c["Emissive"][0], c["Emissive"][1], c["Emissive"][2], 1.f));
        mtrl->SetFloat("g_EmissivePow", c.value("EmissivePow", 1.f));
    }
    
    MaterialContainer[key] = mtrl;
}

Shader* ResourceManager::LoadShader(const std::string& key)
{
    auto iter = ShaderContainer.find(key);

    if (iter != ShaderContainer.end())
        return iter->second;
    else
    {
        auto device = GraphicDevice::GetInstance()->GetDevice();

        LPD3DXEFFECT effect = nullptr;
        LPD3DXBUFFER error = nullptr;
        D3DXCreateEffectFromFile(device, key.c_str(), nullptr, nullptr, 0, nullptr, &effect, &error);

        if (error) {
            OutputDebugStringA((char*)error->GetBufferPointer());
            error->Release();
        }

        auto shader = Shader::Create(effect);

        ShaderContainer[key] = shader;

        return shader;
    }
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

    UITextureContainer[key] = tex;
}

LPDIRECT3DBASETEXTURE9 ResourceManager::LoadTexture(const std::string& key, TEXTURE texType)
{
    auto iter = TextureContainer.find(key);

    if (iter != TextureContainer.end())
        return iter->second;
    else
    {
        LPDIRECT3DBASETEXTURE9 tex = nullptr;
        auto device = GraphicDevice::GetInstance()->GetDevice();

        switch (texType)
        {
        case Engine::TEXTURE::Tex_Diffuse:
            D3DXCreateTextureFromFile(device, key.c_str(), (LPDIRECT3DTEXTURE9*)&tex);
            break;
        case Engine::TEXTURE::Tex_Cube:
            D3DXCreateCubeTextureFromFile(device, key.c_str(), (LPDIRECT3DCUBETEXTURE9*)&tex);
            break;
        default:
            break;
        }

        TextureContainer[key] = tex;

        return tex;
    }
}

Mesh* ResourceManager::GetMesh(const std::string& key)
{
    auto iter = MeshContainer.find(key);

    if (iter != MeshContainer.end())
        return iter->second;
    else
        return nullptr;
}

Material* ResourceManager::GetMaterial(const std::string& key)
{
    auto iter = MaterialContainer.find(key);

    if (iter != MaterialContainer.end())
        return iter->second;
    else
        return nullptr;
}

Shader* ResourceManager::GetShader(const std::string& key)
{
    auto iter = ShaderContainer.find(key);

    if (iter != ShaderContainer.end())
        return iter->second;
    else
        return nullptr;
}

LPDIRECT3DBASETEXTURE9 ResourceManager::GetTexture(const std::string& key)
{
    auto iter = TextureContainer.find(key);

    if (iter != TextureContainer.end())
        return iter->second;
    else
        return nullptr;
}

LPDIRECT3DBASETEXTURE9 ResourceManager::GetTexture(const std::wstring& key)
{
    auto iter = UITextureContainer.find(key);

    if (iter != UITextureContainer.end())
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
