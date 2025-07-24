#include "pch.h"
#include "NPCdolbok.h"

#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "DialogComponent.h"
#include "QuestSystem.h"
#include "UIManager.h"

NPCdolbok::NPCdolbok(ObjectManager* owner, ObjectType objType) :BaseCharacter(owner, objType) {}

NPCdolbok::~NPCdolbok() {}

NPCdolbok* NPCdolbok::Create(ObjectManager* owner, ObjectType objType)
{
    NPCdolbok* Instance = new NPCdolbok(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT NPCdolbok::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    BaseCharacter::Ready_Object(owner, objType);
    InitTransform();
    InitCollision();
    InitPhysics();
    InitDialog();
    return S_OK;
}

void NPCdolbok::Update(_float dt)
{
    BaseCharacter::Update(dt);
}

void NPCdolbok::Late_Update(_float dt)
{
    BaseCharacter::Late_Update(dt);
}

void NPCdolbok::Talk(DialogManager* dialogMgr)
{
    for (auto& set : dialogSets)
    {
        if (set.condition())
        {
            auto* dialog = GetComponent<DialogComponent>();

            dialog->SetDialogLines(set.lines);
            dialog->SetSpeakerName(L"Á¦Çö");

            if (set.onFinish)
                dialog->SetOnFinish(set.onFinish);

            dialogMgr->StartDialog(dialog);
            return;
        }
    }
}

void NPCdolbok::InitTransform()
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);
    transform->SetPosition(60.f, 500.f, 70.f);
    transform->SetRotate(_vec3(0.f, D3DXToRadian(90.f), 0.f));
    SetMaterial("dolbokBody_Mtrl", "Body", RENDER_ID::Render_Alpha);
    SetMaterial("dolbokHead_Mtrl", "Head");
    SetMaterial("dolbokLeftLeg_Mtrl", "LLeg");
    SetMaterial("dolbokRightLeg_Mtrl", "RLeg");
    SetMaterial("dolbokLeftArm_Mtrl", "LArm");
    SetMaterial("dolbokRightArm_Mtrl", "RArm");

    Add_Bone("RHand", ObjectType::Bone, Bones["RArm"], "VillageLeftHand_Mtrl");
    SetScale(_vec3(5.f * Scale, 3.f * Scale, 3.f * Scale), "RHand");
    Bones["RHand"]->GetComponent<TransformComponent>()->Translate(_vec3(-5.f * Scale, -4.f * Scale, -2.f * Scale));

    Add_Bone("LHand", ObjectType::Bone, Bones["LArm"], "VillageRightHand_Mtrl");
    SetScale(_vec3(5.f * Scale, 3.f * Scale, 3.f * Scale), "LHand");
    Bones["LHand"]->GetComponent<TransformComponent>()->Translate(_vec3(5.f * Scale, -4.f * Scale, -2.f * Scale));

    Add_Bone("Nose", ObjectType::Bone, Bones["Head"], "VillageNose_Mtrl");
    SetScale(_vec3(2.f * Scale, 4.f * Scale, 2.f * Scale), "Nose");
    SetPosition(_vec3(0.f, -4.f * Scale, Scale * 8.f), "Nose");
}

void NPCdolbok::InitCollision()
{
    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<AABBCollider>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetSize(_vec3(2.f, 7.f, 2.f));
    collision->SetLayer(LAYER_NPC);
    collision->SetMask(LAYER_PLAYER | LAYER_DEFAULT);
}

void NPCdolbok::InitPhysics()
{
    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);//test
    physics->SetMass(1.f);
}

void NPCdolbok::InitDialog()
{
    
}

void NPCdolbok::Free()
{
    BaseCharacter::Free();
}
