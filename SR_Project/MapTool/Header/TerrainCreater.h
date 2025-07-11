#pragma once
#include "Base.h"
#include "EnginePCH.h"

class TerrainCreater : public Base
{
public:
    TerrainCreater();
    virtual ~TerrainCreater();

public:
    // 높이맵 생성
    void CreateHeightmap(int width, int height, float scale = 0.05f);

    // 블럭 지형 생성
    void CreateBlockTerrain(int terX, int terZ, int terY);

    // 마우스 위치, 일정 반경으로 높이 재조정
    void SetHeight(int cx, int cy, int distance, unsigned char amount);

    void SaveHeightmapAsImage(const std::string& filepath);     // 높이맵 저장하기
    bool LoadHeightmapFromImage(const std::string& filepath);   // 높이맵 불러오기

    const std::vector<SB>& GetBlocks() const { return blocks; }                     // 블럭 정보 가져오기
    const std::vector<unsigned char>& GetHeightmap() const { return heightMap; }    // 높이맵 가져오기
    
    void Free() override;

private:
    // 지형 높이에 따른 블럭 종류 분기
    StaticBlockType GetBlockTypeByHeight(int y, int maxHeight);

private:
    // 지형 넓이와 높이
    int Wid = 0;
    int Hei = 0;

    std::vector<SB> blocks;
    std::vector<unsigned char> heightMap;
};