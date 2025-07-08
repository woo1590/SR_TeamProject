#pragma once
#include "Base.h"
#include "EnginePCH.h"

class TerrainCreater : public Base
{
public:
    TerrainCreater();
    virtual ~TerrainCreater();

    void CreateHeightmap(int width, int height, float scale = 0.05f);
    void SaveHeightmapAsImage(const std::string& filepath);

    bool LoadHeightmapFromImage(const std::string& filepath);
    void CreateBlockTerrain(int maxHeight);

    const std::vector<unsigned char>& GetHeightmap() const { return heightMap; }
    const std::vector<_vec3>& GetBlocks() const { return blocks; }

public:
    void Free() override;

private:
    int width = 0;
    int height = 0;

    std::vector<unsigned char> heightMap;
    std::vector<_vec3> blocks;
};