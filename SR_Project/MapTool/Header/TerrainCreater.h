#pragma once
#include "Base.h"
#include "EnginePCH.h"

class TerrainCreater : public Base
{
private:
    struct PairHash
    {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1, T2>& p) const
        {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };

public:
    TerrainCreater();
    virtual ~TerrainCreater();

public:
    void SaveHeightmapAsImage(const std::string& filepath);     // 높이맵 저장하기
    bool LoadHeightmapFromImage(const std::string& filepath);   // 높이맵 불러오기

    // 높이맵 생성
    void CreateHeightmap(int width, int height, float scale = 0.05f);

    // 블럭 지형 생성
    void CreateBlockTerrain(int terX, int terZ, int terY);

    const std::vector<unsigned char>& GetHeightmap() const { return heightMap; }    // 높이맵 가져오기
    const std::vector<SB>& GetBlocksInChunk(int chunkX, int chunkZ) const;

    void Free() override;

private:
    // 지형 높이에 따른 블럭 종류 분기
    StaticBlockType GetBlockTypeByHeight(int y, int maxHeight);

private:
    // 지형 넓이와 높이
    int Wid = 0;
    int Hei = 0;

    std::vector<unsigned char> heightMap;
    std::unordered_map<std::pair<int, int>, std::vector<SB>, PairHash> chunkBlocks;
};