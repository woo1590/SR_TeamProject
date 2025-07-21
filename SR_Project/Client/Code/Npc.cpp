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
    transform->SetPosition(60.f, 200.f, 60.f);
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
    collision->AddCollider<AABBCollider>();
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
        // [1] 첫 만남 (EquipItem 시작)
        // 덤불 같은 곳에 숨어있다가 플레이어를 보고 겁에 질림
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::NotStarted; },
    {
        {L"…흐읍…!", Emotion::Happy}, // (작게 숨을 삼키는 소리)
        {L"…누, 누구세요? 저리 가요! 몬스터가 당신을 보고 쫓아올지도 몰라요!", Emotion::Sad}, // 두려움에 찬 목소리
        {L"싸, 싸울 수 있는 분인가요? 제발… 뭐라도 단단히 입고 준비해주세요. 보는 제가 불안해서 그래요…", Emotion::Confuse},
    },
    [=] {quest->AcceptQuest(QuestType::EquipItem); }},

    // [2] EquipItem 미완료 리마인드
    // 여전히 두려움에 떨며, 플레이어가 준비되기를 간절히 바람
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::InProgress; },
    {
        {L"아직… 인가요? 이러다간 저희 둘 다 위험해질 거예요…", Emotion::Sad},
        {L"제발 서둘러 주세요… 너무 무서워요…", Emotion::CloseEye},
    }},

    // [3] EquipItem 완료 -> KillMonsters 시작
    // 플레이어가 준비를 마치자, 작은 희망을 갖고 용기를 내어 부탁함
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::KillMonsters) == QuestStatus::NotStarted; },
    {
        {L"…다행이다. 이제 조금은… 아주 조금은 안심이 돼요.", Emotion::Emm}, // 안도의 한숨
        {L"저… 염치없는 부탁이지만… 저희 마을을 습격했던 몬스터들이 근처에 있어요.", Emotion::Sad},
        {L"세 마리만… 세 마리만이라도 물리쳐 주실 수 있나요? 그럼 도망칠 길을 찾을 수 있을 것 같아요…", Emotion::Confuse},
    }, [=] {quest->AcceptQuest(QuestType::KillMonsters); }},

    // [4] KillMonsters 리마인드
    // 플레이어를 보며 희망을 발견하고, 진심으로 응원함
        {[=] {return quest->GetStatus(QuestType::KillMonsters) == QuestStatus::InProgress; },
    {
        {L"굉장해요! 당신이라면 할 수 있을 줄 알았어요!", Emotion::Happy},
        {L"조금만 더요! 제가 여기서 두 손 모아 기도하고 있을게요!", Emotion::CloseEye},
    }},

    // [5] KillMonsters 완료 -> ReachVillage 시작
    // 자신을 구해준 플레이어에게 깊은 감사를 느끼며, 완전히 의지하게 됨
        {[=] {return quest->GetStatus(QuestType::KillMonsters) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::ReachVillage) == QuestStatus::NotStarted; },
    {
        {L"…정말… 정말 다 물리치셨군요… 흑…", Emotion::Sad}, // 안도와 감격의 눈물
        {L"감사합니다… 정말 감사합니다. 당신은 제 생명의 은인이에요.", Emotion::Brave}, // 용기를 내어 똑바로 쳐다보며
        {L"이제 여기서 벗어날 수 있겠어요. 저와 함께… 안전한 곳으로 가주시겠어요?", Emotion::Happy},
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
