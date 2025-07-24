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
#include "CameraComponent.h"
#include "EngineCore.h"
#include "ObjectManager.h"
#include "SoundManager.h"

#define SOUND EngineCore::GetInstance()->GetSoundManager()

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
    
    camComp = GetScene()->GetCameraManager()->GetMainCamera()->GetOwner()->GetComponent<ThirdcamComponent>();
    invComp = owner->GetFrontObject(ObjectType::Player)->GetComponent<InventoryComponent>();

    InitTransform();
    InitCollision();
    InitPhysics();
    InitDialog();

	return S_OK;
}

void Npc::Update(_float dt)
{
    BaseCharacter::Update(dt);

    if (talkCooldown > 0.f)
        talkCooldown -= dt;
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
        {[=] {return quest->GetStatus(QuestType::EquipArmor) == QuestStatus::NotStarted; },
    {
        {L"(고요한 숲 속, 덤불 속에서 미세한 움직임이 느껴진다.)", Emotion::p18},
        {L"(한 소녀가 겁에 질린 채 웅크리고 있다.)", Emotion::p14},
        {L"…누구세요? 저리 가세요! 괴물들이… 당신을 보고 몰려올지도 몰라요…!", Emotion::p12},
        {L"아… 제현님이셨군요. 괴물이 아니어서 정말 다행이에요…", Emotion::p6},
        {L"하지만 그 장비로는 너무 위태로워 보여요. 이 숲은 정말 무서운 곳인데…",Emotion::p3},
        {L"이건… 아버지가 남기신 유품이에요. 저보다는 제현님처럼 용감한 분께 필요할 거예요.", Emotion::p3},
        {L"부디 이걸 입어주세요. 제현님이 무사해야… 저도 희망을 가질 수 있을 것 같아요.", Emotion::p6, [=]() {invComp->Add(ItemType::Armor); }},

    },[=] {quest->AcceptQuest(QuestType::EquipArmor); }},

    // [2] 소녀의 기도 (EquipItem) 진행 중 리마인드
    // 플레이어가 갑옷을 입기를 간절히 바란다
        {[=] {return quest->GetStatus(QuestType::EquipArmor) == QuestStatus::InProgress; },
    {
        {L"그 갑옷… 입어주지 않으실 건가요? 제가 가진 전부인데…",Emotion::p14},
        {L"제현님마저 다치면… 전 정말 혼자가 돼요.",Emotion::p3},
    }},

    // [3] 소녀의 기도 완료 -> 희망의 증명 (KillMonsters) 퀘스트 수락
        {[=] {return quest->GetStatus(QuestType::EquipArmor) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::KillMonsters) == QuestStatus::NotStarted; },
    {
        {L"다행이다… 정말 다행이에요. 이제 조금은 안심이 돼요.",Emotion::p8},
        {L"(그때, 주변에서 몬스터의 울음소리가 섬뜩하게 들려온다.)",Emotion::p14},
        {L"안돼요! 여기까지 쫓아왔나 봐요! 조심하세요!",Emotion::p14},
        {L"근처에 몬스터가 아직 남아있는 것 같아요… 부탁드려요, 제현님! 처치해 주세요!",Emotion::p3},
    }, [=] {quest->AcceptQuest(QuestType::KillMonsters); }},

    // [4] 희망의 증명 (KillMonsters) 진행중 리마인드
    // 자신을 지켜주는 플레이어를 보며 희망을 갖고 응원
        {[=] {return quest->GetStatus(QuestType::KillMonsters) == QuestStatus::InProgress; },
    {
        {L"제현님이 싸우는 모습에서… 희망이 보여요.",Emotion::p7},
        {L"조금만 더 힘내세요! 제가 여기서 기도하고 있을께요!",Emotion::p1},
    }},

    // [5] 희망의 증명 완료 -> 쓸쓸한 귀향 (ReachVillage) 퀘스트 수락
    // 자신을 구해준 플레이어에게 완전히 의지하며, 함께 마을로 가달라고 부탁한다
        {[=] {return quest->GetStatus(QuestType::KillMonsters) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::ReachVillage) == QuestStatus::NotStarted; },
    {
        {L"…정말… 정말 해내셨군요. 흑…", Emotion::p9}, 
        {L"고마워요. 제현님 덕분에…저, 다시 용기를 낼 수 있게 됐어요.", Emotion::p9},
        {L"제현님, 먼저 마을로 가주시겠어요? 저는 여기서 잠시 마음을 추스르고 뒤따라 갈게요.", Emotion::p2},
    },  [=] {quest->AcceptQuest(QuestType::ReachVillage); }},

    // [6] 쓸쓸한 귀향 완료 (마을 도착) -> 새로운 가족 (BuyPig) 퀘스트 수락
    // 폐허가 된 마을에 절망하지만, 떠나려는 상인의 아기 돼지를 보고 새로운 희망을 발견한다.
        {[=] { return quest->GetStatus(QuestType::ReachVillage) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::BuyPig) == QuestStatus::NotStarted; },
        {
        {L"아… 전부… 전부 무너졌어요. 이제 정말… 아무것도 남지 않았네요.", Emotion::p3},
        {L"(그때, 떠나려는 상인의 짐수레에서 아기 돼지의 울음소리가 들린다.)", Emotion::p6, [=]() {SOUND->PlaySFX("Pig"); }},
        {L"어…?! 저 아이… 지금 울고 있었던 건가요?", Emotion::p8},
        {L"부탁이에요. 저 아이를… 우리 마을의 새로운 희망으로 맞아주실 수 없을까요?", Emotion::p3},
        },
        [=] { quest->AcceptQuest(QuestType::BuyPig); }},

        // [7] 새로운 가족 (BuyPig) 완료 -> 대지를 잠재워줘 (KillRedGolem) 퀘스트 수락
        // 아기 돼지와 함께 기뻐하는 순간, 땅의 울림에 공포를 느끼고 그 원인을 제거해달라고 부탁한다.
        {[=] { return quest->GetStatus(QuestType::BuyPig) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::KillRedGolem) == QuestStatus::NotStarted; },
        {
        {L"고마워요! 이제 우리에겐 새로운 가족이 생겼어요!", Emotion::p19},
        {L"(갑자기, 땅이 거칠게 울리며 아기 돼지가 겁에 질려 꿀꿀거린다.)", Emotion::p8, [=]() {camComp->SetShake(12.f, 2.5f); SOUND->PlaySFX("Earthquake"); }},
        {L"이, 이 진동은… 설마…", Emotion::p14},
        {L"틀림없어요. 전설속 '하늘섬'의 수호자, 레드 골렘이 깨어났어요.", Emotion::p12},
        {L"그 분노는 마을을 짓밟고, 주변 생명체를 모조리 파괴할 거예요.", Emotion::p14},
        {L"제발… 골렘을 멈춰주세요. 지금 아니면… 너무 늦을지도 몰라요.", Emotion::p3},
        },
        [=] { quest->AcceptQuest(QuestType::KillRedGolem); }},

        // [8] 대지를 잠재워줘 (KillRedGolem) 완료 -> 악몽의 종언 (KillEnder) 퀘스트 수락
        // 물리적 위협이 사라진 마을에 남은 마지막 공포, '엔더'에 대해 이야기한다.
        {[=] { return quest->GetStatus(QuestType::KillRedGolem) == QuestStatus::Completed &&
        quest->GetStatus(QuestType::KillEnder) == QuestStatus::NotStarted; },
        {
        {L"진동이… 멎었어요! 정말 해내셨군요!", Emotion::p8},
        {L"하지만… 아직 끝이 아니에요. 밤이 되면… 사람들의 마음속 절망을 파고드는 그림자가 나타나요.", Emotion::p3},
        {L"모두가 그 악몽 때문에 돌아오지 못하고 있어요.", Emotion::p14},
        {L"마을의 마지막 공포, '엔더'를 물리쳐주세요.", Emotion::p14},
        },
        [=] { quest->AcceptQuest(QuestType::KillEnder); }},

        // [9] 모든 퀘스트 완료 후
        // 평화를 되찾은 마을에서 플레이어에게 진심으로 감사한다.
        {[=] { return quest->GetStatus(QuestType::KillEnder) == QuestStatus::Completed; },
        {
        {L"보세요! 하늘이 이렇게 맑았던 게 얼마 만인지 모르겠어요.", Emotion::p16},
        {L"제현님이… 이 땅을 짓누르던 기나긴 악몽을 모두 걷어내 주신 덕분이에요. 정말 고마워요.", Emotion::p16},
        {L"비록 모든 것이 부서졌지만… 이제는 슬프지 않아요. 여긴 끝이 아니라, 새로운 시작의 장소니까요.", Emotion::p7},
        {L"분명… 소식을 들은 다른 사람들도 곧 돌아올 거예요. 우리가 희망의 불씨를 다시 피웠으니까요!", Emotion::p1},
        {L"이제부터가 진짜 시작이에요! 함께 이 마을을 다시 일으켜 세워요, 우리의… 영웅, 제현님!", Emotion::p19},
        }},
    };
}

void Npc::Talk(DialogManager* dialogMgr)
{
    if (talkCooldown > 0.f) return;

    for (auto& set : dialogSets)
    {
        if (set.condition())
        {
            auto* dialog = GetComponent<DialogComponent>();

            dialog->SetDialogLines(set.lines);
            dialog->SetSpeakerName(L"우석");

            auto originalOnFinish = set.onFinish;
            dialog->SetOnFinish([this, originalOnFinish]() {
                if (originalOnFinish)
                    originalOnFinish();
                this->talkCooldown = 0.6f;
                });

            dialogMgr->StartDialog(dialog);
            return;
        }
    }
}

void Npc::Free()
{
    BaseCharacter::Free();
}
