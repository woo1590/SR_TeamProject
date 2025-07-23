#include "EnginePCH.h"
#include "Bridge.h"
#include "UIRenderer.h"

//system
#include "EngineCore.h"
#include "ObjectManager.h"
#include "InputSystem.h"
#include "TimerManager.h"
#include "SoundManager.h"

//object
#include "Part.h"
#include "ChunkMesh.h"
#include "CollisionBlock.h";

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"

Bridge::Bridge(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col, int Count)
    : DynamicBlock(owner, objType, type, col, Rot, Count)
{
    AddRef();
}

Bridge::~Bridge()
{
}

Bridge* Bridge::Create(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col, int Count)
{
    Bridge* Instance = new Bridge(owner, objType, type, col, Count);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Bridge::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Object::Ready_Object();
    auto transform = AddComponent<TransformComponent>();

    Parts["BriPar"] = Part::Create(owner, objType, _vec3(0.1f, 0.1f, 0.1f), this, "Stone_Mtrl");
    auto BriPar = Parts["BriPar"]->GetComponent<TransformComponent>();

    const int width = 10;
    const int length = 80;

    float startX = -((width - 1) / 2.f) * 2.f;
    float startZ = 0.f;

    for (int z = 0; z < length; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            std::string name = "Tile_" + std::to_string(z) + "_" + std::to_string(x);
            std::string material;
            _vec3 scale = _vec3(1.f, 1.f, 1.f);
            float heightOffset = 0.f;

            if (x == 0 || x == width - 1)
            {
                material = "PurPilar_Mtrl";
            }
            else if (x == 1 || x == width - 2)
            {
                material = "PurBrick_Half_Mtrl";
                scale.y = 0.5f;
                heightOffset = -0.5f;
            }
            else material = (z % 4 == 0) ? "EndStoneBrick_Mtrl" : "PurBrick_Mtrl";

            Parts[name] = Part::Create(owner, objType, scale, Parts["BriPar"], material);
            auto transform = Parts[name]->GetComponent<TransformComponent>();

            float posX = x * 2.f;
            float posZ = -z * 2.f;
            transform->SetPosition(_vec3(posX, heightOffset, posZ));
        }
    }

    BriPar->SetIsBlock();
    BriPar->SetRotate(D3DXToRadian(90.f), 0.f, 0.f);
    BriPar->Translate(0.f, 0.f, 0.f);

    for (auto& part : Parts) owner->AddObject(ObjectType::Part, part.second);
    return S_OK;
}

void Bridge::Update(_float dt)
{
    if (First)
    {
        DynamicBlock::LoadLink();
        First = false;
    }

    bool allTriggered = !LinkedObject.empty();
    for (auto& Dst : LinkedObject)
    {
        if (!Dst || !static_cast<DynamicBlock*>(Dst)->GetTrigger())
        {
            allTriggered = false;
            break;
        }
    }
    Activate = allTriggered;

    if (Activate && !Trigger)
    {
        Operate(dt);

        if (soundFirst)
        {
            soundFirst = false;
            EngineCore::GetInstance()->GetSoundManager()->PlaySFX("OperateBridge");
        }
    }

    Object::Update(dt);
}

void Bridge::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Bridge::SetMaterial(const std::string& mtrl, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<MeshRenderer>()->SetMaterial(mtrl);
}

void Bridge::SetPosition(_vec3 position, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetPosition(position);
}

void Bridge::SetRotation(_vec3 rotation, string str)
{
    if (Parts[str] != nullptr)
        Parts[str]->GetComponent<TransformComponent>()->SetRotate(rotation);
}

void Bridge::ApplyRotationToBridgeParts(float angleDegree)
{
    float rad = D3DXToRadian(angleDegree);
    for (auto& [name, part] : Parts)
    {
        if (name.find("Bri") != std::string::npos)
        {
            auto trans = part->GetComponent<TransformComponent>();
            trans->SetRotate(rad, 0.f, 0.f);
        }
    }
}

void Bridge::CreateCollisionBlocks()
{
    for (auto& [name, part] : Parts)
    {
        if (name.find("Tile_") != 0) continue;

        auto transform = part->GetComponent<TransformComponent>();
        _vec3 worldPos = transform->GetWorldPosition();

        auto cb = CollisionBlock::Create(owner, ObjectType::StaticBlock);
        cb->GetComponent<TransformComponent>()->SetPosition(worldPos);

        owner->AddObject(ObjectType::StaticBlock, cb);
    }
}

void Bridge::Operate(_float dt)
{
    float targetAngle = 0.f;

    if (totAngle < 90.f)
    {
        Angle = rotSpeed * dt;
        totAngle += Angle;

        if (totAngle > 90.f)
        {
            Angle -= (totAngle - 90.f);
            totAngle = 90.f;
        }

        float currentAngle = 90.f - totAngle;
        ApplyRotationToBridgeParts(currentAngle);
    }
    else
    {
        Trigger = true;
        CreateCollisionBlocks();
    }
}

void Bridge::Free()
{
    DynamicBlock::Free();
}