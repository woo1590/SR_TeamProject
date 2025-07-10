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

void ResourceManager::LoadMesh(const std::string & key, Mesh* mesh)
{
    MeshContainer[key] = mesh;
}

void ResourceManager::LoadMaterial(const std::string& filePath)
{
    using json = nlohmann::json;

    auto baseMtrl = Material::Create();

    json j = json::parse(std::ifstream(filePath.c_str()));

    baseMtrl->SetShader(GetShader(j["template"].value("fx", "BasicShader")));

    if (j["template"].contains("constants"))
    {
        const auto& c = j["template"]["constants"];
        if (c.contains("Diffuse")) baseMtrl->SetVec4("g_Diffuse", _vec4(c["Diffuse"][0], c["Diffuse"][1], c["Diffuse"][2], c["Diffuse"][3]));
        if (c.contains("Specular")) baseMtrl->SetVec4("g_Specular", _vec4(c["Specular"][0], c["Specular"][1], c["Specular"][2],1.f));
        if (c.contains("Shininess")) baseMtrl->SetFloat("g_Shininess", c["Shininess"]);
        if (c.contains("Emissive")) baseMtrl->SetVec4("g_Emissive", _vec4(c["Emissive"][0], c["Emissive"][1], c["Emissive"][2],1.f));
        if (c.contains("EmissivePow")) baseMtrl->SetFloat("g_EmissivePow", c["EmissivePow"]);
        if (c.contains("AlphaCut")) baseMtrl->SetFloat("g_AlphaCut", c["AlphaCut"]);
        if (c.contains("UVScale")) baseMtrl->SetVec4("g_UVScale", _vec4(c["UVScale"][0], c["UVScale"][1], 0.f, 0.f));
        if (c.contains("UVOffset")) baseMtrl->SetVec4("g_UVOffset", _vec4(c["UVOffset"][0], c["UVOffset"][1], 0.f, 0.f));
    }

    for (auto& [name, node] : j["materials"].items())
    {
        Material* mat = baseMtrl->Clone();

        if (node.contains("fx"))
            mat->SetShader(ShaderContainer[node["fx"].get<std::string>()]);

        if(node.contains("textures"))
            for (auto& [slot, path] : node["textures"].items())
            {
                auto tex = (slot == "CubeMap") ? LoadTexture(path, TEXTURE::Tex_Cube) :
                                                 LoadTexture(path, TEXTURE::Tex_Diffuse);

                TextureContainer[path] = tex;
                mat->SetTexture(slot, tex);
            }

        if (node.contains("constants"))
            for (auto& [key, value] : node["constants"].items())
            {
                if (value.is_number_integer())                  mat->SetInt(key, value);
                if (value.is_number_float())                    mat->SetFloat(key, value);
                if (value.is_array() && value.size() == 3)      mat->SetVec3(key, _vec3(value[0], value[1], value[2]));
                if (value.is_array() && value.size() == 4)      mat->SetVec4(key, _vec4(value[0], value[1], value[2], value[3]));
            }

        MaterialContainer[name] = mat;
    }

    Safe_Release(baseMtrl);
}

void ResourceManager::LoadShader(const std::string& filePath, const std::string& key)
{
    auto device = GraphicDevice::GetInstance()->GetDevice();

    LPD3DXEFFECT effect = nullptr;
    LPD3DXBUFFER error = nullptr;
    D3DXCreateEffectFromFile(device, filePath.c_str(), nullptr, nullptr, 0, nullptr, &effect, &error);

    if (error) {
        OutputDebugStringA((char*)error->GetBufferPointer());
        error->Release();
    }

    auto shader = Shader::Create(effect);

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
        return iter->second->Clone();
    else
        return nullptr;
}

Shader* ResourceManager::GetShader(const std::string& key)
{
    auto iter = ShaderContainer.find(key);

    if (iter != ShaderContainer.end())
        return iter->second->Clone();
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

    //std::for_each(TextureContainer.begin(), TextureContainer.end(), [](auto& pair)
    //    {
    //        Safe_Release(pair.second);
    //    });

    std::for_each(ShaderContainer.begin(), ShaderContainer.end(), [](auto& pair)
        {
            Safe_Release(pair.second);
        });
}
