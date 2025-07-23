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
        // [1] 첫 만남 -> 소녀의 기도 (EquipItem) 
        // 숲 속에서 공포에 떨다가 플레이어를 발견하고, 그의 안전을 걱정하며 갑옷을 건넨다
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::NotStarted; },
    {
        {L"(고요한 숲 속, 나무 덤불 속에서 미세한 움직임이 느껴진다.)",Emotion::p18},
        {L"(자세히 보니, 한 소녀가 겁에 질린 채 웅크리고 있다.)",Emotion::p14},
        {L"…흐읍…!", Emotion::p14},
        {L"누, 누구세요…? 저리 가요! 괴물들이 몰려올지도 몰라요…!", Emotion::p12},
        {L"…당신은… 그들과는 다른 것 같네요.", Emotion::p11},
        {L"하지만 그 장비로는 너무 위험해 보여요. 제가… 마을에서 도망칠 때 겨우 챙겨온 갑옷이에요.",Emotion::p3},
        {L"부디 이걸 입어주세요. 당신이 무사해야… 저도 희망을 가질 수 있으니까요.", Emotion::p6},

    },[=] {quest->AcceptQuest(QuestType::EquipItem); }},

    // [2] 소녀의 기도 (EquipItem) 진행 중 리마인드
    // 플레이어가 갑옷을 입기를 간절히 바란다
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::InProgress; },
    {
        {L"그 갑옷... 입어주지 않으실 건가요? 제가 가진 전부인데…",Emotion::p3},
        {L"이 험한 숲에선 한순간의 방심이 목숨을 앗아갈 수 있어요…",Emotion::p2},
    }},

    // [3] 소녀의 기도 완료 -> 희망의 증명 (KillMonsters) 퀘스트 수락
        {[=] {return quest->GetStatus(QuestType::EquipItem) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::KillMonsters) == QuestStatus::NotStarted; },
    {
        {L"다행이다… 이제 조금은 안심이 돼요.",Emotion::p8},
        {L"(주변에서 몬스터의 울음소리가 들려온다.)",Emotion::p14},
        {L"안돼요! 여기까지 쫓아왔어요! 조심하세요!",Emotion::p14},
    }, [=] {quest->AcceptQuest(QuestType::KillMonsters); }},

    // [4] 희망의 증명 (KillMonsters) 진행중 리마인드
    // 자신을 지켜주는 플레이어를 보며 희망을 갖고 응원
        {[=] {return quest->GetStatus(QuestType::KillMonsters) == QuestStatus::InProgress; },
    {
        {L"굉장해요! 당신이라면 할 수 있을 줄 알았어요!",Emotion::p8},
        {L"조금만 더 힘내세요! 제가 여기서 기도하고 있을께요!",Emotion::p1},
    }},

    // [5] 희망의 증명 완료 -> 쓸쓸한 귀향 (ReachVillage) 퀘스트 수락
    // 자신을 구해준 플레이어에게 완전히 의지하며, 함께 마을로 가달라고 부탁한다
        {[=] {return quest->GetStatus(QuestType::KillMonsters) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::ReachVillage) == QuestStatus::NotStarted; },
    {
        {L"…정말… 정말 해내셨군요. 흑…", Emotion::p9}, 
        {L"고마워요. 당신 덕분에 용기가 생겼어요.", Emotion::p9},
        {L"함께 가주시겠어요? 폐허가 되어버렸지만… 저의 유일한 집인 마을로요.", Emotion::p17},
    },  [=] {quest->AcceptQuest(QuestType::ReachVillage); }},

    // [6] 쓸쓸한 귀향 완료 (마을 도착) -> 새로운 가족 (BuyPig) 퀘스트 수락
    // 폐허가 된 마을에 절망하지만, 떠나려는 상인의 아기 돼지를 보고 새로운 희망을 발견한다.
        {[=] { return quest->GetStatus(QuestType::ReachVillage) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::BuyPig) == QuestStatus::NotStarted; },
        {
        {L"아… 아아… 전부… 전부 무너졌어요. 이제 정말 다 끝이야…", Emotion::p3},
        {L"(떠나려는 상인의 짐수레에서 아기 돼지의 울음소리가 들린다.)", Emotion::p6},
        {L"저기… 저 아이 좀 보세요. 이 잿더미 속에서도 살아있어요.", Emotion::p8},
        {L"부탁이에요. 저 아이를… 우리 마을의 새로운 가족으로 맞아주실 수 없을까요?", Emotion::p3},
        },
        [=] { quest->AcceptQuest(QuestType::BuyPig); }},

        // [7] 새로운 가족 (BuyPig) 완료 -> 대지를 잠재워줘 (KillRedGolem) 퀘스트 수락
        // 아기 돼지와 함께 기뻐하는 순간, 땅의 울림에 공포를 느끼고 그 원인을 제거해달라고 부탁한다.
        {[=] { return quest->GetStatus(QuestType::BuyPig) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::KillRedGolem) == QuestStatus::NotStarted; },
        {
        {L"고마워요! 이제 우리에겐 새로운 가족이 생겼어요!", Emotion::p19},
        {L"(땅이 크게 흔들리며 아기 돼지가 겁에 질려 꿀꿀거린다.)", Emotion::p8},
        {L"이, 이 진동은…! '하늘섬'의 '레드 골렘'이 틀림없어요.", Emotion::p14},
        {L"저 분노의 울음이 멎지 않는 한, 우리 가족은 편히 쉴 수 없어요. 제발… 골렘을 멈춰주세요!", Emotion::p3},
        },
        [=] { quest->AcceptQuest(QuestType::KillRedGolem); }},

        // [8] 대지를 잠재워줘 (KillRedGolem) 완료 -> 악몽의 종언 (KillEnder) 퀘스트 수락
        // 물리적 위협이 사라진 마을에 남은 마지막 공포, '엔더'에 대해 이야기한다.
        {[=] { return quest->GetStatus(QuestType::KillRedGolem) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::KillEnder) == QuestStatus::NotStarted; },
        {
        {L"진동이… 멎었어요! 정말 해내셨군요!", Emotion::p16},
        {L"하지만… 아직 끝이 아니에요. 밤이 되면… 사람들의 마음속 절망을 파고드는 그림자가 나타나요.", Emotion::p11},
        {L"모두가 그 악몽 때문에 돌아오지 못하고 있어요. 마을의 마지막 공포, '엔더'를 물리쳐주세요.", Emotion::p7},
        },
        [=] { quest->AcceptQuest(QuestType::KillEnder); }},

        // [9] 모든 퀘스트 완료 후
        // 평화를 되찾은 마을에서 플레이어에게 진심으로 감사한다.
        {[=] { return quest->GetStatus(QuestType::KillEnder) == QuestStatus::Completed; },
        {
        // 1. 플레이어를 안심시킨다
        {L"당신 덕분에… 우리 마을은 다시 살아났어요.", Emotion::p16},
        {L"정말… 정말 고마워요.", Emotion::p9},

        // 2. 가까이 다가와 감사를 표하는 척하며 찌른다.
        {L"(아리아가 플레이러를 껴안는 척하며 날카로운 무언가로 찌른다.)",Emotion::p23},

        // 3. 충격적인 진실을 밝히낟
        {L"골렘은 대지의 마지막 저항이었고, 엔더는 떠도는 영혼들의 원념이었죠. 당신은 스스로 제물이 될 무대를 청소한 셈이예요.",Emotion::p22},
        {L"엔더까지 물리친 당신의 영혼이라면… '그분'도 분명 만족하시겠죠.",Emotion::p22},
        {L"이제… 이게 진짜 시작이에요. 우리의… 영원한 마을.",Emotion::p5},
        }},
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
