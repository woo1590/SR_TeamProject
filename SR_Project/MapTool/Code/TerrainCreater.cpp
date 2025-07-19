#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    this->Wid = width;
    this->Hei = height;
    heightMap.resize(width * height);

    float randomZ = static_cast<float>(rand()) / RAND_MAX * 1000.f;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float noise = stb_perlin_noise3(x * scale, y * scale, randomZ, 0, 0, 0);
            noise = (noise + 1.0f) * 0.5f;
            heightMap[y * width + x] = static_cast<unsigned char>(noise * 255.f);
        }
    }
}

void TerrainCreater::SaveHeightmapAsImage(const std::string& filename)
{
    int width = Wid;
    int height = Hei;

    std::string filepath = "../Resource/HeightMap/" + filename + ".png";
    stbi_write_png(filepath.c_str(), width, height, 1, heightMap.data(), width);
}

bool TerrainCreater::LoadHeightmapFromImage(const std::string& filename)
{
    std::string filepath = "../Resource/HeightMap/" + filename + ".png";

    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 1);

    if (!data) return false;

    heightMap.resize(width * height);
    memcpy(heightMap.data(), data, width * height);

    stbi_image_free(data);

    this->Wid = width;
    this->Hei = height;

    return true;
}

void TerrainCreater::CreateBlockTerrain(int terX, int terZ, int terY)
{
    chunkBlocks.clear();

    int blocksX = terX * (CHUNK_SIZE / BLOCK_SIZE);
    int blocksZ = terZ * (CHUNK_SIZE / BLOCK_SIZE);

    auto GetHeightIndex = [&](int x, int z)
        {
            int mapX = min(x, Wid - 1);
            int mapZ = min(z, Hei - 1);
            return heightMap[mapZ * Wid + mapX];
        };

    for (int z = 0; z < blocksZ; ++z)
    {
        for (int x = 0; x < blocksX; ++x)
        {
            unsigned char heightValue = GetHeightIndex(x, z);
            int blockHeight = (heightValue * terY) / 255;

            int topY(-1);
            bool isExist(false);

            for (int y = 0; y < terY; ++y)
            {
                if (y >= blockHeight) break;

                StaticBlockData block;

                block.Pos =
                {
                    x * BLOCK_SIZE + BLOCK_SIZE * 0.5f,
                    y * BLOCK_SIZE + BLOCK_SIZE * 0.5f,
                    z * BLOCK_SIZE + BLOCK_SIZE * 0.5f
                };
                block.Axis = StaticBlockAxis::sAY;
                block.Rot = StaticBlockRot::sREnd;
                block.Usage = StaticBlockUsage::Basic;

                if (y <= 4) block.Type = Air;
                else
                {
                    isExist = true;
                    block.Type = GetBlockTypeByHeight(y, terY);
                }

                topY = y;

                int chunkX = x / (CHUNK_SIZE / BLOCK_SIZE);
                int chunkZ = z / (CHUNK_SIZE / BLOCK_SIZE);
                chunkBlocks[{chunkX, chunkZ}].push_back(block);
            }

            if (topY != -1 && isExist)
            {
                int chunkX = x / (CHUNK_SIZE / BLOCK_SIZE);
                int chunkZ = z / (CHUNK_SIZE / BLOCK_SIZE);

                auto& vec = chunkBlocks[{chunkX, chunkZ}];
                vec.back().Type = StaticBlockType::DarkGrass;
            }
        }
    }
}

StaticBlockType TerrainCreater::GetBlockTypeByHeight(int y, int maxHeight)
{
    if (y <= 1) return DarkDirt;
    else if (y < maxHeight * 0.3f) return DarkDirt;
    else if (y < maxHeight * 0.55f) return DarkStone;
    else if (y < maxHeight * 0.75f) return DarkDirt;
    else if (y < maxHeight * 0.85f) return DarkStone;
    else return DarkDirt;
}

const std::vector<SB>& TerrainCreater::GetBlocksInChunk(int chunkX, int chunkZ) const
{
    auto it = chunkBlocks.find({ chunkX, chunkZ });
    return it->second;
}

void TerrainCreater::Free()
{
    heightMap.clear();
    heightMap.shrink_to_fit();
}