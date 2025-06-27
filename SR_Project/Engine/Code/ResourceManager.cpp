#include "EnginePCH.h"
#include "ResourceManager.h"
#include "GraphicDevice.h"
#include "Mesh.h"
#include "Material.h"
#include "TerrainMesh.h"
#include "StaticMesh.h"
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

void ResourceManager::LoadTerrain(const std::wstring& filePath, const std::wstring& key, _float cellSpacing, _float heightScale)
{
	auto terrain = TerrainMesh::Create();
	terrain->LoadTerrain(filePath,cellSpacing,heightScale);

	TerrainContainer[key] = terrain;
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

TerrainMesh* ResourceManager::GetTerrain(const std::wstring& key)
{
	auto iter = TerrainContainer.find(key);

	if (iter != TerrainContainer.end())
		return iter->second;
	else
		return nullptr;
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

    std::for_each(TerrainContainer.begin(), TerrainContainer.end(), [](auto& pair)
        {
            Safe_Release(pair.second);
        });
}
