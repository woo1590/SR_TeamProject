#include "EnginePCH.h"
#include "Chunk.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"

Chunk::Chunk(int chunkX, int chunkZ) : CD(chunkX, chunkZ)
{
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
        // Object* obj = StaticBlock::Create(objectMgr, ObjectType::StaticBlock, sb.Type, sb.Dir);
        // obj->GetComponent<TransformComponent>()->SetPosition(sb.Pos);
        // objectMgr->AddObject(ObjectType::StaticBlock, obj);
    }

    for (const auto& db : CD.dBlocks)
    {
        Object* obj = DynamicBlock::Create(objectMgr, ObjectType::DynamicBlock, db.Type, db.Dir, Count);
        obj->GetComponent<TransformComponent>()->SetPosition(db.Pos);
        objectMgr->AddObject(ObjectType::DynamicBlock, obj);
    }
}

void Chunk::Free()
{
}