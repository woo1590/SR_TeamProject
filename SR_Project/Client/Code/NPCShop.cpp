#include "pch.h"
#include "NPCShop.h"

#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "DialogComponent.h"
#include "QuestSystem.h"
#include "UIManager.h"

NPCShop::NPCShop(ObjectManager* owner, ObjectType objType) :BaseCharacter(owner, objType) {}

NPCShop::~NPCShop() {}
NPCShop* NPCShop::Create(ObjectManager* owner, ObjectType objType)
{
    NPCShop* Instance = new NPCShop(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT NPCShop::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    BaseCharacter::Ready_Object(owner, objType);
    InitTransform();
    InitCollision();
    InitPhysics();
    InitDialog();
    return S_OK;
}

void NPCShop::Update(_float dt)
{
    BaseCharacter::Update(dt);
}

void NPCShop::Late_Update(_float dt)
{
    BaseCharacter::Late_Update(dt);
}

void NPCShop::Talk(DialogManager* dialogMgr)
{
    for (auto& set : dialogSets)
    {
        if (set.condition())
        {
            auto* dialog = GetComponent<DialogComponent>();

            dialog->SetDialogLines(set.lines);
            dialog->SetSpeakerName(L"Àç¿í");

            if (set.onFinish)
                dialog->SetOnFinish(set.onFinish);

            dialogMgr->StartDialog(dialog);
            return;
        }
    }
}

void NPCShop::InitTransform()
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);
    transform->SetPosition(60.f, 500.f, 80.f);
    transform->SetRotate(_vec3(0.f, D3DXToRadian(90.f), 0.f));
    SetMaterial("shopBody_Mtrl", "Body", RENDER_ID::Render_Alpha);
    SetMaterial("shopHead_Mtrl", "Head");
    SetMaterial("shopLeftLeg_Mtrl", "LLeg");
    SetMaterial("shopRightLeg_Mtrl", "RLeg");
    SetMaterial("shopLeftArm_Mtrl", "LArm");
    SetMaterial("shopRightArm_Mtrl", "RArm");
}

void NPCShop::InitCollision()
{
    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<AABBCollider>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetSize(_vec3(2.f, 7.f, 2.f));
    collision->SetLayer(LAYER_NPC);
    collision->SetMask(LAYER_PLAYER | LAYER_DEFAULT);
}

void NPCShop::InitPhysics()
{
    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);//test
    physics->SetMass(1.f);
}

void NPCShop::InitDialog()
{
   
}

void NPCShop::Free()
{
    BaseCharacter::Free();
}
