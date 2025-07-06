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

void Chunk::AddBlock(ObjectManager* objectMgr, int Count)
{
    for (const auto& sb : CD.sBlocks)
    {
        Object* obj = StaticBlock::Create(objectMgr, ObjectType::StaticBlock, sb.Type, sb.Axis, sb.Rot, sb.Usage);
        obj->GetComponent<TransformComponent>()->SetPosition(sb.Pos);
        objectMgr->AddObject(ObjectType::StaticBlock, obj);
    }
}

void Chunk::Free()
{
    CD.sBlocks.clear();
}