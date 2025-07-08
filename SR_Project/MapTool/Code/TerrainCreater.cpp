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

    this->width = width;
    this->height = height;

    return true;
}

void TerrainCreater::CreateBlockTerrain(int maxHeight)
{
    blocks.clear();

    for (int z = 0; z < height; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            unsigned char heightValue = heightMap[z * width + x];
            int blockHeight = (heightValue * maxHeight) / 255;

            for (int y = 0; y < blockHeight; ++y)
                blocks.push_back({ (float)x, (float)y, (float)z });
        }
    }
}

void TerrainCreater::Free()
{
    heightMap.clear();
    heightMap.shrink_to_fit();
}