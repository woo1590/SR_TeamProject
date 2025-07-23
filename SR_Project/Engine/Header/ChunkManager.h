#pragma once
#include "Base.h"

BEGIN(Engine)
class Chunk;
class Scene;    
class ENGINE_DLL ChunkManager : public Base
{
public:     //ûũ�δ����� �����ϱ� ���� public���� ��
    struct PairHash
    {
        size_t operator()(const std::pair<int, int>& key) const { return std::hash<int>()(key.first) ^ (std::hash<int>()(key.second) << 1); }
    };

private:
    ChunkManager(Scene* owner);
    virtual ~ChunkManager();

public:
    static ChunkManager* Create(Scene* owner);

public:
    Chunk* CreateChunk(int chunkX, int chunkZ);     // Key ���� ûũ ������ �ش� ûũ ��ȯ, ������ ���� �� ��ȯ
    void RemoveChunk(int chunkX, int chunkZ);       // Key ���� ûũ ������ ����
    void ClearAllChunks();                          // ��� ûũ Ŭ���� �� �ϳ��� ûũ�� ����� �޸� ����

    void SaveChunk(const std::wstring& saveStage);
    void LoadChunk(const std::wstring& loadStage, bool isEditor = false);
    
    void IsChunkBoundary(_vec3 playerPos);                                        // Player�� ûũ ��踦 �Ѿ�����
    void UpdateRenderChunk(int playerChunkX, int playerChunkZ, int count);        // ûũ �ε� (Player�� ���� ûũ �߽� 3 x 3 ûũ�� ������)

    Chunk* GetChunk(int chunkX, int chunkZ);                                      // Key ���� ûũ ������ �ش� ûũ ��ȯ
    const unordered_map<std::pair<int, int>, Chunk*, PairHash>& GetChunks() const { return worldChunks; }

    /*-----------------Only Client-------------------------*/
    void SetChunk(std::unordered_map<std::pair<int, int>, Chunk*, PairHash>& chunks);
    void SetChunkRange(_uint range) { chunkRange = range; }
    void CreateMiniMapChunk(int chunkX, int chunkZ, Chunk* chunk, SceneID sceneID);
    bool GetMiniMapChunk(int chunkX, int chunkZ, MINIMAP& outData);

private:
    void Free()override;

private:
    int preChunkX = -1, preChunkZ = -1;
    Scene* owner = nullptr;
    std::vector<std::pair<int, int>> renderChunks;
    std::unordered_map<std::pair<int, int>, Chunk*, PairHash> worldChunks;
    _uint chunkRange = 7;
    std::unordered_map<std::pair<int, int>, MINIMAP, PairHash> miniMapChunks;
};
END