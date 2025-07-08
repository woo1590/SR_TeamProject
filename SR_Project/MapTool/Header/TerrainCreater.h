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
    void CreateBlockTerrain(int terrainWidth, int terrainDepth, int maxHeight);

    const std::vector<unsigned char>& GetHeightmap() const { return heightMap; }
    const std::vector<SB>& GetBlocks() const { return blocks; }

private:
    StaticBlockType GetBlockTypeByHeight(int y, int maxHeight);

public:
    void Free() override;

private:
    int hWidth = 0;
    int hHeight = 0;

    std::vector<unsigned char> heightMap;
    std::vector<SB> blocks;
};