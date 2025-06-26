#include "EnginePCH.h"
#include "ResourceManager.h"
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

void ResourceManager::LoadTexture(const wstring& key, const wstring& path)
{
    LPDIRECT3DTEXTURE9 texture = nullptr;
    auto device = GraphicDevice::GetInstance()->GetDevice();
    if (SUCCEEDED(D3DXCreateTextureFromFileW(device, path.c_str(), &texture)))
        TextureContainer[key] = texture;
}

LPDIRECT3DTEXTURE9 ResourceManager::GetTexture(const wstring& key)
{
    auto it = TextureContainer.find(key);
    return it != TextureContainer.end() ? it->second : nullptr;
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

	std::for_each(TerrainContainer.begin(), TerrainContainer.end(), [](auto& pair)
		{
			Safe_Release(pair.second);
		});

    for (auto& p : TextureContainer)
        Safe_Release(p.second);
    TextureContainer.clear();
}
