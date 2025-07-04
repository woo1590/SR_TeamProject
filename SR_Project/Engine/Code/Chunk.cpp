#include "EnginePCH.h"
#include "Chunk.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"

Chunk::Chunk(CHUNK chunkData) : CD(chunkData)
{
}

Chunk::~Chunk()
{
}

Chunk* Chunk::Create(CHUNK chunkData)
{
    Chunk* Instance = new Chunk(chunkData);

    if (FAILED(Instance->Ready_Object(chunkData)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }

    return Instance;
}

void Chunk::AddBlock(ObjectManager* objectMgr, int Count)
{
    for (const auto& sb : CD.sBlocks)
    {
        Object* obj = StaticBlock::Create(objectMgr, ObjectType::StaticBlock, sb.Type, sb.Dir);
        obj->GetComponent<TransformComponent>()->SetPosition(sb.Pos);
        objectMgr->AddObject(ObjectType::StaticBlock, obj);
    }

    for (const auto& db : CD.dBlocks)
    {
        Object* obj = DynamicBlock::Create(objectMgr, ObjectType::DynamicBlock, db.Type, db.Dir, Count);
        obj->GetComponent<TransformComponent>()->SetPosition(db.Pos);
        objectMgr->AddObject(ObjectType::DynamicBlock, obj);
    }
}

HRESULT Chunk::Ready_Object(CHUNK chunkData)
{
    CD = chunkData;
    return S_OK;
}

void Chunk::Free()
{
}