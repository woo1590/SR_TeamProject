#include "pch.h"
#include "Scene.h"
#include "Monster.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "InfoComponent.h"
#include "InfoDetector.h"
#include "ProgressBar.h"
#include "EnemyHPBarFront.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "EngineCore.h"
#include "GraphicDevice.h"
#include "Bone.h"
#include "EnemyHPBarBack.h"
#include "ExpBarBack.h"
#include "BossHPBarFront.h"
#include "BossIcon.h"
#include "ParticleObj.h"
#include "WorldUIComponent.h"
#include "DamageText.h"
#include "Fontcomponent.h"
#include "HPBarWhite.h"
#include "UIManager.h"
#include "QuestSystem.h"

Monster::Monster(ObjectManager* owner, ObjectType objType)
	:BaseCharacter(owner, objType)
{
}

Monster::~Monster()
{
}

HRESULT Monster::Ready_Object(ObjectManager* owner, ObjectType objType, MonsterType type)
{
    BaseCharacter::Ready_Object(owner, objType);

    auto  statcomponent = AddComponent<InfoComponent<EnemyInfo>>();
    auto playerInfo = owner->GetFrontObject(ObjectType::Player)->GetComponent<InfoComponent<PlayerInfo>>();
    if (!playerInfo) 
        return E_FAIL;

    auto collision = AddComponent<CollisionComponent>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetLayer(LAYER_ENEMY);
    collision->SetMask(LAYER_PLAYER | LAYER_DEFAULT | LAYER_PROJECTILE | LAYER_ENEMY);
    collision->SetCollisionStay([this](Object* other) {this->OnCollisionStay(other); });
    
    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);//test
    physics->SetMass(1.f);

    // --------------------------------------------

    if (type == MonsterType::Monster)
    {
        auto body = dynamic_cast<Bone*>(Bones["Body"]);
        assert(body && "Bones[Body] missing");
        auto monsterTf = body->GetComponent<TransformComponent>();
    
        enemyFront = EnemyHPBarFront::Create(owner);
        statcomponent->Attach(enemyFront->GetComponent<ProgressBar<EnemyInfo>>());
        enemyFront->SetTarget(monsterTf);
        owner->AddUIObject(enemyFront);
    
        enemyBack = EnemyHPBarBack::Create(owner);
        auto frontTf = enemyFront->GetComponent<TransformComponent>();
        auto backTf = enemyBack->GetComponent<TransformComponent>();

        whiteBack = HPBarWhite::Create(owner);
        auto* backBar = whiteBack->GetComponent<ProgressBar<EnemyInfo>>();
        statcomponent->Attach(backBar);

        whiteBack->GetComponent<TransformComponent>()->SetParent(frontTf);
        backTf->SetParent(frontTf);
        backTf->SetPosition({0.f, 0.f, 0.f});
        backTf->SetScale({0.2f, 0.2f, 1.f});
    
        enemyFront->AddChild(enemyBack);
        enemyFront->AddChild(whiteBack);
       
        owner->AddUIObject(enemyBack);
        owner->AddUIObject(whiteBack);

        statcomponent->SetOnZeroHp([=]() 
            {
                auto quest = GetScene()->GetUIManager()->GetQuestSystem();
                if (quest)
                {
                    quest->ReportQuestProgress(QuestType::KillMonsters, 1);
                    playerInfo->AddExp(5);
                }
            });
    }
    else if (type == MonsterType::Boss)
    {
        bossFront = BossHPBarFront::Create(owner);
        auto bossHpComp = bossFront->GetComponent<ProgressBar<EnemyInfo>>();
        bossHpComp->AppearAnimation(2.f);
        statcomponent->Attach(bossHpComp);
        owner->AddUIObject(bossFront);

        bossBack = ExpBarBack::Create(owner);
        bossBack->GetComponent<TransformComponent>()->SetPosition(350.f, 100.f);
        bossBack->GetComponent<TransformComponent>()->SetScale(2.38f, 3.f);
        owner->AddUIObject(bossBack);

        whiteBack = HPBarWhite::Create(owner);
        auto* whiteBar = whiteBack->GetComponent<ProgressBar<EnemyInfo>>();
        statcomponent->Attach(whiteBar);

        auto whiteTf = whiteBack->GetComponent<TransformComponent>();
        whiteTf->SetParent(bossFront);
        whiteTf->SetPosition(0.f, 0.f); 
        whiteTf->SetScale(4.75f, 1.5f);

        bossIcon = BossIcon::Create(owner);
        auto bossIconTf = bossIcon->GetComponent<TransformComponent>();
        bossIconTf->SetParent(bossFront);
       
        owner->AddUIObject(bossIcon);
        owner->AddUIObject(whiteBack);
        
        bossFront->AddChild(bossBack); 
        bossFront->AddChild(whiteBack);
        bossFront->AddChild(bossIcon);

        statcomponent->SetOnZeroHp([=]()
            {
                auto quest = GetScene()->GetUIManager()->GetQuestSystem();
                if (quest)
                {
                    quest->ReportQuestProgress(QuestType::KillRedGolem, 1);
                    playerInfo->AddExp(15);
                }
            });
    }
    IsHit = nullptr;

    return S_OK;
}

void Monster::Update(_float dt)
{
    BaseCharacter::Update(dt);
}

void Monster::Late_Update(_float dt)
{
    BaseCharacter::Late_Update(dt);
}

void Monster::MoveTo(_vec3* dir, _float dt)
{
}

void Monster::RotateTo(_vec3* dir, float dt)
{
}

void Monster::Attack(Object* target)
{
}

void Monster::Die()
{
    
}

void Monster::DeleteBar()
{
    Object* bars[] = {enemyFront, enemyBack, bossFront, bossBack, whiteBack, bossIcon};
    for (auto* bar : bars)
        if (bar)
            bar->SetDead();
}

void Monster::ShowDmgText(int dmg, const _vec3& hitDir)
{
    auto headTf = Bones["Head"]->GetComponent<TransformComponent>();
    if (!headTf) return;

    auto dmgText = DamageText::Create(owner);

    dmgText->worldPos = headTf->GetWorldPosition();
    dmgText->screenDir = WorldUIComponent::ToScreen(hitDir, ScreenMode::Direction);

    dmgText->GetComponent<FontComponent>()->entries[0].text = to_wstring(dmg);

    owner->AddUIObject(dmgText);
}


void Monster::Hit(_vec3 dir, _float power)
{
    auto stat = GetComponent<InfoComponent<EnemyInfo>>();
    ShowDmgText(stat->GetInfo().power, dir);
}

_float Monster::GetHp()
{
    auto  statcomponent = GetComponent<InfoComponent<EnemyInfo>>();
    return statcomponent->GetInfo().curHp;
}

void Monster::BackStep(_vec3* dir, _float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    auto Stat = GetComponent<InfoComponent<EnemyInfo>>();

    if (State != MonsterState::Walk) State = MonsterState::Walk;
    D3DXVec3Normalize(dir, dir);
    Transform->SetForward(*dir * -1);
    Transform->Translate(*dir * dt * Stat->GetInfo().speed);
}

void Monster::SetHit(_bool Hit)
{
    if (IsHit != nullptr) *IsHit = Hit;
}

_bool Monster::GetHit()
{
    if (IsHit != nullptr)
        return *IsHit;
    else
        return false;
}

void Monster::InitAnimation()
{
   
}

void Monster::PlayAnimation(_float dt)
{
}

void Monster::PlayIdle(_float dt)
{
    //idle animation
}

void Monster::PlayWalk(_float dt)
{
}

void Monster::PlayAttack(_float dt)
{
}

void Monster::PlayDie(_float dt)
{
}

void Monster::PlayHit(_float dt)
{
}

void Monster::OnCollisionStay(Object* other)
{
    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();

    //if (objType == ObjectType::StaticBlock)
    //    collision->ResolveAABBColiision(other);
}

void Monster::Free()
{
    Safe_Delete(Distance);
    Safe_Delete(IsAttack);
    Safe_Delete(IsHit);
    BaseCharacter::Free();
}