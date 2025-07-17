#include "pch.h"
#include "Npc.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "DialogComponent.h"
#include "QuestSystem.h"
#include "UIManager.h"

Npc::Npc(ObjectManager* owner, ObjectType objType)
	:BaseCharacter(owner, objType)
{
}

Npc::~Npc()
{
}

Npc* Npc::Create(ObjectManager* owner, ObjectType objType)
{
    Npc* Instance = new Npc(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Npc::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    BaseCharacter::Ready_Object(owner, objType);
    InitTransform();
    InitCollision();
    InitPhysics();
    InitDialog();
	return S_OK;
}

void Npc::Update(_float dt)
{
    BaseCharacter::Update(dt);
}

void Npc::Late_Update(_float dt)
{
    BaseCharacter::Late_Update(dt);
}

void Npc::InitTransform()
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);
    transform->SetPosition(30.f, 30.f, 30.f);
    transform->SetRotate(_vec3(0.f, D3DXToRadian(90.f), 0.f));
    SetMaterial("VillageBody_Mtrl", "Body", RENDER_ID::Render_Alpha);
    SetScale(_vec3(8.f * Scale, 16.f * Scale, 4.f * Scale), "Body");
    SetMaterial("VillageHead_Mtrl", "Head");
    SetMaterial("VillageLeg_Mtrl", "LLeg");
    SetMaterial("VillageLeg_Mtrl", "RLeg");
    SetMaterial("VillageArm_Mtrl", "LArm");
    SetScale(_vec3(3.f* Scale, 6.f * Scale, 3.f * Scale), "LArm");
    SetRotation(_vec3(D3DXToRadian(-45.f), 0.f, 0.f), "LArm");
    SetMaterial("VillageArm_Mtrl", "RArm");
    SetScale(_vec3(3.f * Scale, 6.f * Scale, 3.f * Scale), "RArm");
    SetRotation(_vec3(D3DXToRadian(-45.f), 0.f, 0.f), "RArm");

    Add_Bone("RHand", ObjectType::Bone, Bones["RArm"], "VillageLeftHand_Mtrl");
    SetScale(_vec3(5.f * Scale, 3.f * Scale, 3.f * Scale), "RHand");
    Bones["RHand"]->GetComponent<TransformComponent>()->Translate(_vec3(-5.f * Scale, -4.f* Scale, -2.f * Scale));
    
    Add_Bone("LHand", ObjectType::Bone, Bones["LArm"], "VillageRightHand_Mtrl");
    SetScale(_vec3(5.f * Scale, 3.f * Scale, 3.f * Scale), "LHand");
    Bones["LHand"]->GetComponent<TransformComponent>()->Translate(_vec3(5.f * Scale, -4.f * Scale, -2.f * Scale));

    Add_Bone("Nose", ObjectType::Bone, Bones["Head"], "VillageNose_Mtrl");
    SetScale(_vec3(2.f * Scale, 4.f * Scale, 2.f * Scale), "Nose");
    SetPosition(_vec3(0.f, -4.f * Scale, Scale * 8.f), "Nose");
}

void Npc::InitCollision()
{
    auto collision = AddComponent<CollisionComponent>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetSize(_vec3(2.f, 7.f, 2.f));
    collision->SetLayer(LAYER_NPC);
    collision->SetMask(LAYER_PLAYER | LAYER_DEFAULT );
}

void Npc::InitPhysics()
{
    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);//test
    physics->SetMass(1.f);
}

void Npc::InitDialog()
{
    auto quest = GetScene()->GetUIManager()->GetQuestSystem();
    auto dialog = AddComponent<DialogComponent>();

    dialogSets = {
        // EquipItem 시작
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::NotStarted; },
        {
            L"까아~ 어세오세요, 제현님!",
            L"우선 장비 하나 장착해보세요! (인벤토리 열어서 아무거나요~)"
        },
        [=] {quest->AcceptQuest(QuestType::EquipItem); }},
        // EquipItem 미완료 리마인드
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::InProgress; },
        {
            L"제가 장비 장착하라고 했잖아요. 까먹으셨어요? (인벤토리 버튼 눌러보세요!)"
        },
        []() {}},
        // EquipItem 완료 -> killMonsters 시작
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::KillMonsters) == QuestStatus::NotStarted; },
        {
            L"오! 멋져요! 이제 몬스터가 문제네요...",
            L"근처 몬스터 세 마리만 물리쳐 주실 수 있을까요..?"
        }, [=] {quest->AcceptQuest(QuestType::KillMonsters); }},
        // KillMonsters 리마인드
        {[=] {return quest->GetStatus(QuestType::KillMonsters) == QuestStatus::InProgress; },
        {
            L"몬스터 세 마리만 잡아주면 돼요! 힘내세요!"
        },
        []() {}},
        // KillMonsters 완료 -> ReachVillage 시작
        {[=] {return quest->GetStatus(QuestType::KillMonsters) == QuestStatus::Completed
        && quest->GetStatus(QuestType::ReachVillage) == QuestStatus::NotStarted; },
        {
            L"역시 제현님! 이젠 마을로 돌아가 볼까요?",
            L"모두 다들 기다리고 있어요~"   
        }, [=] {quest->AcceptQuest(QuestType::ReachVillage); }},
    };
}

void Npc::Talk(DialogManager* dialogMgr)
{
    for (auto& set : dialogSets)
    {
        if (set.condition())
        {
            auto* dialog = GetComponent<DialogComponent>();

            dialog->SetDialogLines(set.lines);
            dialog->SetSpeakerName(L"우석");

            if (set.onFinish)
                dialog->SetOnFinish(set.onFinish);

            dialogMgr->StartDialog(dialog);
            return;
        }
    }
}

void Npc::Free()
{
    BaseCharacter::Free();
}
