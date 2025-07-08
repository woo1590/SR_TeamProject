#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Include/stb_image_write.h"

#define STB_PERLIN_IMPLEMENTATION
#include "../Include/stb_perlin.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../Include/stb_image.h"

#include "TerrainCreater.h"

TerrainCreater::TerrainCreater()
{
    srand(static_cast<unsigned int>(time(0)));
}

TerrainCreater::~TerrainCreater()
{
}

void TerrainCreater::CreateHeightmap(int width, int height, float scale)
{
    heightMap.resize(width * height);

    float randomZ = static_cast<float>(rand()) / RAND_MAX * 1000.0f;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float noise = stb_perlin_noise3(x * scale, y * scale, randomZ, 0, 0, 0);
            noise = (noise + 1.0f) * 0.5f;
            heightMap[y * width + x] = static_cast<unsigned char>(noise * 255.0f);
        }
    }
}

void TerrainCreater::SaveHeightmapAsImage(const std::string& filename)
{
    std::string filepath = "../Resource/HeightMap/" + filename + ".png";

    int width = static_cast<int>(sqrt(heightMap.size()));
    int height = width;

    stbi_write_png(filepath.c_str(), width, height, 1, heightMap.data(), width);
}

bool TerrainCreater::LoadHeightmapFromImage(const std::string& filename)
{
    std::string filepath = "../Resource/HeightMap/" + filename + ".png";

    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 1);

    if (!data)
        return false;

    heightMap.resize(width * height);
    memcpy(heightMap.data(), data, width * height);

    stbi_image_free(data);

    this->Wid = width;
    this->Hei = height;

    return true;
}

void TerrainCreater::CreateBlockTerrain(int terrainWidth, int terrainDepth, int maxHeight)
{
    blocks.clear();
    blocks.resize(terrainWidth * terrainDepth * maxHeight);

    auto GetIndex = [&](int x, int y, int z) { return y * terrainDepth * terrainWidth + z * terrainWidth + x; };

    for (int z = 0; z < terrainDepth; ++z)
    {
        for (int x = 0; x < terrainWidth; ++x)
        {
            int mapX = min(x, Wid - 1);
            int mapZ = min(z, Hei - 1);

            unsigned char heightValue = heightMap[mapZ * Wid + mapX];
            int blockHeight = (heightValue * maxHeight) / 255;

            for (int y = 0; y < maxHeight; ++y)
            {
                int index = GetIndex(x, y, z);

                StaticBlockData block;
                block.Pos = { x * 2.f, y * 2.f, z * 2.f };
                block.Axis = StaticBlockAxis::sAY;
                block.Rot = StaticBlockRot::sREnd;
                block.Usage = StaticBlockUsage::Basic;

                if (y < blockHeight) block.Type = GetBlockTypeByHeight(y, maxHeight);
                else block.Type = StaticBlockType::Air;

                blocks[index] = block;
            }
        }
    }
}

StaticBlockType TerrainCreater::GetBlockTypeByHeight(int y, int maxHeight)
{
    if (y <= 1)
        return StaticBlockType::Stone;
    else if (y < maxHeight * 0.4f)
        return StaticBlockType::Stone;
    else if (y < maxHeight * 0.9f)
        return StaticBlockType::Dirt;
    else
        return StaticBlockType::GrassDirt;
}

void TerrainCreater::Free()
{
    blocks.clear();
    blocks.shrink_to_fit();
    heightMap.clear();
    heightMap.shrink_to_fit();
}