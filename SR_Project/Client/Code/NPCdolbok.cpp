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
            dialog->SetSpeakerName(L"제현");

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
    auto quest = GetScene()->GetUIManager()->GetQuestSystem();
    auto dialog = AddComponent<DialogComponent>();

    dialogSets = {
        // EquipItem 시작
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::NotStarted; },
        {
        {L"까아~ 어서오세요, 제현님!", Emotion::Happy},
        {L"이런 낯선 곳까지 오시다니, 정말 대단해요! 사실 제가 길을 잃어서 곤란하던 참이었거든요.", Emotion::Emm},
        {L"우선 장비 하나 장착해보세요! (인벤토리 열어서 아무거나요~)", Emotion::Happy},
        },
        [=] {quest->AcceptQuest(QuestType::EquipItem); }},

        // EquipItem 미완료 리마인드
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::InProgress; },
        {
        {L"에이, 아직도요? 제 말은 귓등으로 들으셨나 봐요! 흥!", Emotion::Emm},
        {L"험한 곳이라구요! 어서 인벤토리에서 장비 하나만이라도 꺼내 입어주세요!", Emotion::CloseEye},
        }},

        // EquipItem 완료 -> killMonsters 시작
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::KillMonsters) == QuestStatus::NotStarted; },
        {
        {L"오! 훨씬 든든해 보여요! 역시 제현님!", Emotion::Brave},
        {L"그런데... 저 숲에서 자꾸 이상한 소리가 들려와서 무서워요.", Emotion::Sad},
        {L"저 때문에 위험에 빠트리는 것 같아 미안하지만... 근처 몬스터 세마리만 물리쳐 주실 수 있을까요?", Emotion::Confuse},
        }, [=] {quest->AcceptQuest(QuestType::KillMonsters); }},

        // KillMonsters 리마인드
        {[=] {return quest->GetStatus(QuestType::KillMonsters) == QuestStatus::InProgress; },
        {
        {L"조금만 더요! 제현님이라면 문제없을 거예요", Emotion::Happy},
        {L"제가 여기서 열심히 응원하고 있어요!!", Emotion::CloseEye},
        }},

        // KillMonsters 완료 -> ReachVillage 시작
        {[=] {return quest->GetStatus(QuestType::KillMonsters) == QuestStatus::Completed
        && quest->GetStatus(QuestType::ReachVillage) == QuestStatus::NotStarted; },
        {
        {L"세상에! 정말 몬스터들을 다 물리치셨군요! 역시 저의 영웅님!", Emotion::Brave},
        {L"이제 안심하고 마을로 갈 수 있겠어요. 이젠 저와 함께 마을로 돌아가 볼까요?", Emotion::Happy},
        {L"마을에 가면 제 친구들도 제현님을 보고 싶어 할 거예요", Emotion::Happy},
        }, [=] {quest->AcceptQuest(QuestType::ReachVillage); }},
    };
}

void NPCdolbok::Free()
{
    BaseCharacter::Free();
}
