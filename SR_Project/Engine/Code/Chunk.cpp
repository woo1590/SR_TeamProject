#include "EnginePCH.h"
#include "Chunk.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"

Chunk::Chunk(int chunkX, int chunkZ)
{
    CD.chunkX = chunkX;
    CD.chunkZ = chunkZ;
}

Chunk::~Chunk()
{
}

Chunk* Chunk::Create(int chunkX, int chunkZ)
{
    Chunk* Instance = new Chunk(chunkX, chunkZ);
    return Instance;
}

void Chunk::AddBlock(const _vec3& pos, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage)
{
    SB block;
    block.Pos = pos;
    block.Type = type;
    block.Axis = axis;
    block.Rot = rot;
    block.Usage = usage;
    CD.sBlocks.push_back(block);
}

void Chunk::Render()
{
   
}

void Chunk::Free()
{
    CD.sBlocks.clear();
}