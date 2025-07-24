#include "pch.h"
#include "Ender.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "BlackBoard.h"
#include "BehaviorTree.h"
#include "AIController.h"
#include "ChangeState.h"
#include "StateAttack.h"
#include "CheckStateChangeCount.h"
#include "IsChangingState.h"
#include "IsRandomMoveCheck.h"
#include "IsTimerOn.h"
#include "IsAlive.h"
#include "SelectorNode.h"
#include "Die.h"
#include "LaserHead.h"
#include "LaserEffect.h"
#include "ObjectManager.h"
#include "EnderProjectile.h"
#include "FireBlock.h"
#include "EngineCore.h"
#include "SoundManager.h"
#include "InfoComponent.h"
#include "BossHPBarFront.h"
#include "UIManager.h"
#include "QuestSystem.h"
#include "Object.h"
#include "scene.h"
#include "Material.h"
#include "PhysicsComponent.h"
#include "EnderDead.h"
#include "DeadEffect.h"

Ender::Ender(ObjectManager* owner, ObjectType objType)
	:Boss(owner, objType)
{
}

Ender::~Ender()
{
}

Ender* Ender::Create(ObjectManager* owner, ObjectType objType)
{
    Ender* Instance = new Ender(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Ender::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Boss::Ready_Object(owner, objType);

    InitTransform(objType);
    InitTree();
    InitAnimation();

    auto collision = GetComponent<CollisionComponent>();
    collision->AddCollider<OBBCollider>();
    collision->SetSize(_vec3(5.f, 20.f, 5.f));

    auto Info = GetComponent<InfoComponent<EnemyInfo>>();
    Info->SetInfo({ 9, 1000, 1000, 0,0,20, 0,0 });

    CrawlToStand();
    InitCrossLaser();
    InitEnderProjectile();

    bossFront->SetBossName(L"엔더의 심장");
	return S_OK;
}

void Ender::Update(_float dt)
{
    Boss::Update(dt);
    PlayAnimation(dt);

    auto stat = GetComponent<InfoComponent<EnemyInfo>>()->GetInfo();
    if (stat.curHp <= 0) Die();
}

void Ender::Late_Update(_float dt)
{
    Boss::Late_Update(dt);
}

void Ender::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    Add_Bone("TopHead", objType, Bones["Head"], "EnderTopHead_Mtrl");
    SetMaterial("EnderBottomHead_Mtrl", "Head", RENDER_ID::Render_NonAlpha);
    SetMaterial("EnderBody_Mtrl", "Body", RENDER_ID::Render_NonAlpha);

    transform->SetPosition(10 , 100, 10);
    auto toptransform = Bones["TopHead"]->GetComponent<TransformComponent>();
    Bones["TopHead"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_NonAlpha);
    toptransform->SetScale(24.f * Scale, 18.f * Scale, 24.f * Scale);
    toptransform->SetPosition(0.0f, 24.f * Scale, 0.f);
    toptransform->SetPivot(0.f, -18.f* Scale, 24.f * Scale);
    toptransform->SetPivotEnable(true);
   /* toptransform->SetRotate(D3DXToRadian(-45.f), 0.f, 0.f);*/

    auto bottomtransform = Bones["Head"]->GetComponent<TransformComponent>();
    bottomtransform->SetScale(24.f * Scale, 6.f * Scale, 24.f * Scale);
    bottomtransform->SetPosition(0.f * Scale, 66.f * Scale, 0.f * Scale);
    bottomtransform->SetPivot(0.f, -84.f * Scale,0.f);
    bottomtransform->SetPivotEnable(true);
    bottomtransform->SetRotate(_vec3(0.f, 0.f, 0.f));

    auto bodytransform = Bones["Body"]->GetComponent<TransformComponent>();
    bodytransform->SetScale(16.f * Scale, 60.f * Scale, 16.f * Scale);

    Add_Bone("LLeg1", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto leg1transrorm = Bones["LLeg1"]->GetComponent<TransformComponent>();
    leg1transrorm->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    leg1transrorm->SetPosition(40.f* Scale, 45.f* Scale, 0 * Scale);
    leg1transrorm->SetPivot(-24.f * Scale, -45.f * Scale, 0.f);
    leg1transrorm->SetPivotEnable(true);
    leg1transrorm->SetRotate(0.f, D3DXToRadian(45.f), 0.f);

    Add_Bone("LLeg2", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto leg2transform = Bones["LLeg2"]->GetComponent<TransformComponent>();
    leg2transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    leg2transform->SetPosition(40.f * Scale, 0 * Scale, 0 * Scale);
    leg2transform->SetPivot(-24.f * Scale, 0.f, 0.f);
    leg2transform->SetPivotEnable(true);
    leg2transform->SetRotate(0.f, D3DXToRadian(45.f), 0.f);

    Add_Bone("LLeg3", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto leg3transform = Bones["LLeg3"]->GetComponent<TransformComponent>();
    leg3transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    leg3transform->SetPosition(40.f * Scale, -45.f * Scale, 0 * Scale);
    leg3transform->SetPivot(-24.f * Scale, 45.f * Scale, 0.f);
    leg3transform->SetPivotEnable(true);
    leg3transform->SetRotate(0.f, D3DXToRadian(45.f), 0.f);

    Add_Bone("LFoot1", ObjectType::Bone, Bones["LLeg1"], "EnderBody_Mtrl");
    auto foot1transform = Bones["LFoot1"]->GetComponent<TransformComponent>();
    foot1transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    foot1transform->SetPosition(54.f * Scale, 0.f, 0 * Scale);
    foot1transform->SetPivot(-30.f * Scale, 0.f, 0.f);
    foot1transform->SetPivotEnable(true);
    foot1transform->SetRotate(0.f, D3DXToRadian(-90.f), 0.f);

    Add_Bone("LFoot2", ObjectType::Bone, Bones["LLeg2"], "EnderBody_Mtrl");
    auto foot2transform = Bones["LFoot2"]->GetComponent<TransformComponent>();
    foot2transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    foot2transform->SetPosition(54.f * Scale, 0.f, 0 * Scale);
    foot2transform->SetPivot(-30.f * Scale, 0.f, 0.f);
    foot2transform->SetPivotEnable(true);
    foot2transform->SetRotate(0.f, D3DXToRadian(-90.f), 0.f);

    Add_Bone("LFoot3", ObjectType::Bone, Bones["LLeg3"], "EnderBody_Mtrl");
    auto foot3transform = Bones["LFoot3"]->GetComponent<TransformComponent>();
    foot3transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    foot3transform->SetPosition(54.f * Scale, 0.f, 0 * Scale);
    foot3transform->SetPivot(-30.f * Scale, 0.f, 0.f);
    foot3transform->SetPivotEnable(true);
    foot3transform->SetRotate(0.f, D3DXToRadian(-90.f), 0.f);
 
    Add_Bone("RLeg1", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto rleg1transrorm = Bones["RLeg1"]->GetComponent<TransformComponent>();
    rleg1transrorm->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    rleg1transrorm->SetPosition(-40.f * Scale, 45.f * Scale, 0 * Scale);
    rleg1transrorm->SetPivot(24.f * Scale, -45.f * Scale, 0.f);
    rleg1transrorm->SetPivotEnable(true);
    rleg1transrorm->SetRotate(0.f, D3DXToRadian(-45.f), 0.f);

    Add_Bone("RLeg2", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto rleg2transform = Bones["RLeg2"]->GetComponent<TransformComponent>();
    rleg2transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    rleg2transform->SetPosition(-40.f * Scale, 0 * Scale, 0 * Scale);
    rleg2transform->SetPivot(24.f * Scale, 0.f, 0.f);
    rleg2transform->SetPivotEnable(true);
    rleg2transform->SetRotate(0.f, D3DXToRadian(-45.f), 0.f);

    Add_Bone("RLeg3", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto rleg3transform = Bones["RLeg3"]->GetComponent<TransformComponent>();
    rleg3transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    rleg3transform->SetPosition(-40.f * Scale, -45.f * Scale, 0 * Scale);
    rleg3transform->SetPivot(24.f * Scale, 45.f * Scale, 0.f);
    rleg3transform->SetPivotEnable(true);
    rleg3transform->SetRotate(0.f, D3DXToRadian(-45.f), 0.f);

    Add_Bone("RFoot1", ObjectType::Bone, Bones["RLeg1"], "EnderBody_Mtrl");
    auto rfoot1transform = Bones["RFoot1"]->GetComponent<TransformComponent>();
    rfoot1transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    rfoot1transform->SetPosition(-54.f * Scale, 0.f, 0 * Scale);
    rfoot1transform->SetPivot(30.f * Scale, 0.f, 0.f);
    rfoot1transform->SetPivotEnable(true);
    rfoot1transform->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
    
    Add_Bone("RFoot2", ObjectType::Bone, Bones["RLeg2"], "EnderBody_Mtrl");
    auto rfoot2transform = Bones["RFoot2"]->GetComponent<TransformComponent>();
    rfoot2transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    rfoot2transform->SetPosition(-54.f * Scale, 0.f, 0 * Scale);
    rfoot2transform->SetPivot(30.f * Scale, 0.f, 0.f);
    rfoot2transform->SetPivotEnable(true);
    rfoot2transform->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
    
    Add_Bone("RFoot3", ObjectType::Bone, Bones["RLeg3"], "EnderBody_Mtrl");
    auto rfoot3transform = Bones["RFoot3"]->GetComponent<TransformComponent>();
    rfoot3transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    rfoot3transform->SetPosition(-54.f * Scale, 0.f, 0 * Scale);
    rfoot3transform->SetPivot(30.f * Scale, 0.f, 0.f);
    rfoot3transform->SetPivotEnable(true);
    rfoot3transform->SetRotate(0.f, D3DXToRadian(90.f), 0.f);

    Bones["LArm"]->SetDead();
    Bones["LArm"] = nullptr;

    Bones["RArm"]->SetDead();
    Bones["RArm"] = nullptr;

    Bones["LLeg"]->SetDead();
    Bones["LLeg"] = nullptr;

    Bones["RLeg"]->SetDead();
    Bones["RLeg"] = nullptr;

    materials.push_back(GetMaterial("Head"));
    materials.push_back(GetMaterial("Body"));
    materials.push_back(GetMaterial("TopHead"));
    materials.push_back(GetMaterial("LLeg1"));
    materials.push_back(GetMaterial("LLeg2"));
    materials.push_back(GetMaterial("LLeg3"));
    materials.push_back(GetMaterial("LFoot1"));
    materials.push_back(GetMaterial("LFoot2"));
    materials.push_back(GetMaterial("LFoot3"));
    materials.push_back(GetMaterial("RLeg1"));
    materials.push_back(GetMaterial("RLeg2"));
    materials.push_back(GetMaterial("RLeg3"));
    materials.push_back(GetMaterial("RFoot1"));
    materials.push_back(GetMaterial("RFoot2"));
    materials.push_back(GetMaterial("RFoot3"));

    for (auto& material : materials)
    {
        material->SetFloat("alpha", 1);
    }
}

void Ender::InitTree()
{
    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    ChangeStateCount = new int(2);
    bb->SetValue("TriggerCount", ChangeStateCount);
    IsAttack = new _bool(false);
    bb->SetValue("IsAttack", IsAttack);
    IsDie = new _bool(false);
    bb->SetValue("IsDie", IsDie);
    auto transform = GetComponent<TransformComponent>();
    _vec3 startPos = transform->GetPosition();
    TargetPos = new _vec3(startPos.x + 30, startPos.y, startPos.z + 40); //startpos setting->initialize after
    bb->SetValue("targetPos", TargetPos);

    CheckStateChangeCount* changeCount = new CheckStateChangeCount(new StateAttackNode());
    SelectorNode* behaviorNode = new SelectorNode();
    behaviorNode->AddChild(changeCount);
    behaviorNode->AddChild(new ChangeStateNode());

    IsRandomMoveCheck* randomMove = new IsRandomMoveCheck(behaviorNode);
    
    IsAliveNode* isAlive = new IsAliveNode(randomMove);

    SelectorNode* root = new SelectorNode();
    root->AddChild(isAlive);

    BehaviorTree* bt = BehaviorTree::Create(root);

    auto AI = AddComponent<AIController>(bt, bb);
}

void Ender::InitAnimation()
{
    CrawlAnim.ElapsedTime = 0.f;

    StandAnim.ElapsedTime = 0.f;

    CrawlToStandAnim.Start = 0.f;
    CrawlToStandAnim.End = 89.f;
    CrawlToStandAnim.TotalTime = 0.7f;
    CrawlToStandAnim.ElapsedTime = 0.f;

    StandToCrawlAnim.Start = 0.f;
    StandToCrawlAnim.End = 89.f;
    StandToCrawlAnim.TotalTime = 0.7f;
    StandToCrawlAnim.ElapsedTime = 0.f;

    HideAnim.ElapsedTime = 0.f;
    HideAnim.TotalTime = 1.5f;

    SproutAnim.ElapsedTime = 0.f;
    SproutAnim.TotalTime = 1.5f;

    HeadAttackAnim.DelayTime = 2.f;
    HeadAttackAnim.ElapsedTime = 0.f;

    LaserAttackAnim.ElapsedTime = 0.f;
    LaserAttackAnim.TotalTime = 8.f;

    ProjectileAttackAnim.ElapsedTime = 0.f;
    ProjectileAttackAnim.TotalTime = 1.f;
    ProjectileAttackAnim.Start = 0.f;
    ProjectileAttackAnim.End = -45.f;

    DieAnim.ElapsedTime = 0.f;
    DieAnim.TotalTime = 7.5f;
}

void Ender::PlayAnimation(_float dt)
{
    switch (enderState)
    {
    case EnderState::CrawlToStand:
        PlayCrawlToStand(dt);
        break;
    case EnderState::StandToCrawl:
        PlayStandToCrawl(dt);
        break;
    case EnderState::Crawl:
        PlayCrawl(dt);
        break;
    case EnderState::Stand:
        PlayStand(dt);
        break;
    case EnderState::Hidden:
        PlayHide(dt);
        break;
    case EnderState::Sprout:
        PlaySprout(dt);
        break;
    case EnderState::LineLaser:
        PlayLineLaserAttack(dt);
        break;
    case EnderState::CrossLaser:
        PlayCrossLaserAttack(dt);
        break;
    case EnderState::Projectile:
        PlayProjectileAttack(dt);
        break;
    case EnderState::Die:
        PlayDie(dt);
        break;
    }
}

void Ender::Free()
{
    Boss::Free();
}

void Ender::SetTargetPos(_vec3 pos)
{
    *TargetPos = pos;
}

void Ender::Crawl()
{
    if (enderState != EnderState::Crawl)
    {
        enderState = EnderState::Crawl;

        auto transform = Bones["Body"]->GetComponent<TransformComponent>();
        transform->SetRotate(_vec3(D3DXToRadian(89.f),0.f,0.f ));

        auto bottomtransform = Bones["Head"]->GetComponent<TransformComponent>();
        bottomtransform->SetRotate(_vec3(D3DXToRadian(-90.f), 0.f, 0.f));
    }
}

void Ender::Stand()
{
    if (enderState != EnderState::Stand)
    {
        enderState = EnderState::Stand;

        auto transform = Bones["Body"]->GetComponent<TransformComponent>();
        transform->SetRotate(_vec3(0.f, 0.f, 0.f));

        auto bottomtransform = Bones["Head"]->GetComponent<TransformComponent>();
        bottomtransform->SetRotate(_vec3(0.f, 0.f, 0.f));

        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(90.f), 0.f }, "RFoot1");
        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(90.f), 0.f }, "RFoot2");
        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(90.f), 0.f }, "RFoot3");

        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(-90.f), 0.f }, "LFoot1");
        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(-90.f), 0.f }, "LFoot2");
        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(-90.f), 0.f }, "LFoot3");

        *IsAttack = false;
    }
}

void Ender::Hide()
{
    if (enderState != EnderState::Hidden)
    {
        *IsDie = true;
        auto transform = Bones["Body"]->GetComponent<TransformComponent>();
        HideStartY = transform->GetPosition().y;
        enderState = EnderState::Hidden;

        HideAnim.ElapsedTime = 0.f;
        ++CurChangeStateCount;
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("HiddenEnder");
    }
}

void Ender::HideIdle()
{
    if (enderState != EnderState::HideIdle)
    {
        enderState = EnderState::HideIdle;
    }
}

void Ender::Sprout()
{
    if (enderState != EnderState::Sprout)
    {
        enderState = EnderState::Sprout;

        SproutAnim.ElapsedTime = 0.f;
        ++CurChangeStateCount;
        for (auto& Bone : Bones)
        {
            if (Bone.second == nullptr) continue;
            auto renderer = Bone.second->GetComponent<MeshRenderer>();
            renderer->SetRenderID(RENDER_ID::Render_NonAlpha);
        }
        IsSproutPlay = false;
    }
}

void Ender::CrawlToStand()
{
    if (enderState != EnderState::CrawlToStand)
    {
        enderState = EnderState::CrawlToStand;

        CrawlToStandAnim.ElapsedTime = 0.f;
        ++CurChangeStateCount;
    }
}

void Ender::StandToCrawl()
{
    if (enderState != EnderState::StandToCrawl)
    {
        enderState = EnderState::StandToCrawl;

        StandToCrawlAnim.ElapsedTime = 0.f;
        ++CurChangeStateCount;
    }
}

void Ender::LineLaserAttack()
{
    if (enderState != EnderState::LineLaser)
    {
        enderState = EnderState::LineLaser;

        HeadAttackAnim.ElapsedTime = 0.f;
        HeadAttackAnim.DelayTime = 2.f;
    }
}

void Ender::CrossLaserAttack()
{
    if (enderState != EnderState::CrossLaser)
    {
        enderState = EnderState::CrossLaser;

        LaserAttackAnim.ElapsedTime = 0.f;

        for (auto& CrossLaser : CrossLasers)
            CrossLaser->SetActive(true);
    }
}

void Ender::ProjectileAttack()
{
    if (enderState != EnderState::Projectile)
    {
        enderState = EnderState::Projectile;

        ProjectileAttackAnim.ElapsedTime = 0.f;

        auto pos = GetComponent<TransformComponent>()->GetPosition();
        _vec3 playerpos = owner->GetFrontObject(ObjectType::Player)->GetComponent<TransformComponent>()->GetPosition();
        pos.y = 0;
        playerpos.y = 0;
        _vec3 dir = playerpos - pos;
        GetComponent<TransformComponent>()->SetForward(dir);
    }
}

void Ender::MoveTo(_vec3 targetPos, _float dt)
{
    auto transform = GetComponent<TransformComponent>();
    _vec3 pos = transform->GetPosition();

    FireSpawnTime += dt;
    if (FireSpawnTime > 0.2f)
    {
        auto fire = FireBlock::Create(owner, ObjectType::SpriteEffect);
        fire->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y - 9.f, pos.z);
        fire->SetActive(true);
        fire->SetDeadTime(3.f);
        owner->AddObject(ObjectType::SpriteEffect, fire);

        FireSpawnTime = 0.f;
    }

    pos.y = 0;
    _vec3 dir = targetPos - pos;
    dir.y = 0;

    if (D3DXVec3Length(&dir) >= 0.1)
    {
        D3DXVec3Normalize(&dir, &dir);
        transform->Translate(dir * dt * 15.f);
        transform->SetForward(dir);
    }
}

void Ender::Die()
{
    if (enderState != EnderState::Die)
    {
        enderState = EnderState::Die;
        GetScene()->GetUIManager()->GetQuestSystem()->ReportQuestProgress(QuestType::KillEnder, 1);
        DieAnim.ElapsedTime = 0.f;
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("DeathEnder");
        for (auto& laser : CrossLasers) laser->SetDead();
        {
            Bones["TopHead"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["Head"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["Body"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["LLeg1"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["LLeg2"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["LLeg3"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["LFoot1"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["LFoot2"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["LFoot3"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["RLeg1"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["RLeg2"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["RLeg3"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["RFoot1"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["RFoot2"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
            Bones["RFoot3"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
        }
    }
}

void Ender::SetState(EnderState state)
{
    enderState = state;
}

EnderState Ender::GetState()
{
    return enderState;
}

int Ender::GetCurChangeStateCount()
{
    return CurChangeStateCount;
}

void Ender::InitCrossLaser()
{
    for (int i = 0; i < 4; ++i)
    {
        auto laser = LaserEffect::Create(owner, ObjectType::Bone);
        CrossLasers.push_back(laser);

        auto Lasertransform = laser->GetComponent<TransformComponent>();
        Lasertransform->SetScale(_vec3(80.f * Scale, 0.5f, 1.f));
        Lasertransform->SetParent(Bones["Body"]);
        Lasertransform->SetPosition(_vec3(0.f, -60.f * Scale, 0.f));
        Lasertransform->SetPivot(0.f, 60.f * Scale, 0.f);
        Lasertransform->SetPivotEnable(true);
        auto collision = laser->GetComponent<CollisionComponent>();
        collision->SetSize(_vec3(80.f, 1.f, 1.f));
        laser->SetActive(false);

        owner->AddObject(ObjectType::ParticleEffect, laser);
    }

    CrossLasers[0]->GetComponent<TransformComponent>()->SetRotate(_vec3(0.f, D3DXToRadian(90.f),0.f));
    CrossLasers[1]->GetComponent<TransformComponent>()->SetRotate(_vec3(0.f, D3DXToRadian(180.f), 0.f));
    CrossLasers[2]->GetComponent<TransformComponent>()->SetRotate(_vec3(0.f, D3DXToRadian(270.f),0.f));
    CrossLasers[3]->GetComponent<TransformComponent>()->SetRotate(_vec3(0.f, D3DXToRadian(0.f), 0.f ));
}

void Ender::InitEnderProjectile()
{
    EnderProjectiles.reserve(30);

    for (int i = 0; i < 30; ++i)
        EnderProjectiles.push_back(EnderProjectile::Create(owner, ObjectType::Projectile));
}

void Ender::PlayCrawl(_float dt)
{
    CrawlAnim.ElapsedTime += dt;

    WalkTime += dt;
    if (WalkTime > 0.3f)
    {
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkEnder");
        WalkTime = 0.f;
    }

    float LegSwingAngle = D3DXToRadian(30.f);
    float AnimSpeed = 6.f; 

    float phaseLLeg1 = 0.f;
    float phaseRLeg2 = D3DXToRadian(120.f); 
    float phaseLLeg3 = D3DXToRadian(240.f); 

    float phaseRLeg1 = D3DXToRadian(180.f); 
    float phaseLLeg2 = D3DXToRadian(300.f);
    float phaseRLeg3 = D3DXToRadian(60.f);

    float angleLLeg1 = sinf(CrawlAnim.ElapsedTime * AnimSpeed + phaseLLeg1);
    float angleRLeg2 = sinf(CrawlAnim.ElapsedTime * AnimSpeed + phaseRLeg2);
    float angleLLeg3 = sinf(CrawlAnim.ElapsedTime * AnimSpeed + phaseLLeg3);
                                                  
    float angleRLeg1 = sinf(CrawlAnim.ElapsedTime * AnimSpeed + phaseRLeg1);
    float angleLLeg2 = sinf(CrawlAnim.ElapsedTime * AnimSpeed + phaseLLeg2);
    float angleRLeg3 = sinf(CrawlAnim.ElapsedTime * AnimSpeed + phaseRLeg3);

    SetRotation({ LegSwingAngle * angleLLeg1, D3DXToRadian(45.f), 0.f }, "LLeg1");
    SetRotation({ LegSwingAngle * angleRLeg2, D3DXToRadian(-45.f), 0.f }, "RLeg2");
    SetRotation({ LegSwingAngle * angleLLeg3, D3DXToRadian(45.f), 0.f }, "LLeg3");

    SetRotation({ LegSwingAngle * angleRLeg1, D3DXToRadian(-45.f), 0.f }, "RLeg1");
    SetRotation({ LegSwingAngle * angleLLeg2, D3DXToRadian(45.f), 0.f }, "LLeg2");
    SetRotation({ LegSwingAngle * angleRLeg3, D3DXToRadian(-45.f), 0.f }, "RLeg3");
}

void Ender::PlayCrawlToStand(_float dt)
{
    CrawlToStandAnim.ElapsedTime += dt;

    _float t = clamp(CrawlToStandAnim.ElapsedTime / CrawlToStandAnim.TotalTime, 0.f, 1.f);
    float angle = lerp(CrawlToStandAnim.Start, CrawlToStandAnim.End, t);

    auto transform = Bones["Body"]->GetComponent<TransformComponent>();
    transform->SetRotate(_vec3(D3DXToRadian(90.f - angle), 0.f, 0.f));

    auto bottomtransform = Bones["Head"]->GetComponent<TransformComponent>();
    bottomtransform->SetRotate(_vec3(D3DXToRadian(-90.f + angle), 0.f, 0.f));

    SetRotation({ D3DXToRadian(-angle), D3DXToRadian(-90.f), 0.f }, "LFoot1");
    SetRotation({ D3DXToRadian(-angle), D3DXToRadian(-90.f), 0.f }, "LFoot2");
    SetRotation({ D3DXToRadian(-angle), D3DXToRadian(-90.f), 0.f }, "LFoot3");

    SetRotation({ D3DXToRadian(-angle), D3DXToRadian(90.f), 0.f }, "RFoot1");
    SetRotation({ D3DXToRadian(-angle), D3DXToRadian(90.f), 0.f }, "RFoot2");
    SetRotation({ D3DXToRadian(-angle), D3DXToRadian(90.f), 0.f }, "RFoot3");
   
    if (CrawlToStandAnim.ElapsedTime > CrawlToStandAnim.TotalTime)
    {
        Stand();
    }
}

void Ender::PlayStandToCrawl(_float dt)
{
    StandToCrawlAnim.ElapsedTime += dt;

    _float t = clamp(StandToCrawlAnim.ElapsedTime / StandToCrawlAnim.TotalTime, 0.f, 1.f);
    float angle = lerp(StandToCrawlAnim.Start, StandToCrawlAnim.End, t);

    auto transform = Bones["Body"]->GetComponent<TransformComponent>();
    transform->SetRotate(_vec3(D3DXToRadian(angle), 0.f, 0.f));

    auto bottomtransform = Bones["Head"]->GetComponent<TransformComponent>();
    bottomtransform->SetRotate(_vec3(D3DXToRadian(-angle), 0.f, 0.f));

    SetRotation({ D3DXToRadian(-90.f + angle), D3DXToRadian(-90.f), 0.f }, "LFoot1");
    SetRotation({ D3DXToRadian(-90.f + angle), D3DXToRadian(-90.f), 0.f }, "LFoot2");
    SetRotation({ D3DXToRadian(-90.f + angle), D3DXToRadian(-90.f), 0.f }, "LFoot3");

    SetRotation({ D3DXToRadian(-90.f + angle), D3DXToRadian(90.f), 0.f }, "RFoot1");
    SetRotation({ D3DXToRadian(-90.f + angle), D3DXToRadian(90.f), 0.f }, "RFoot2");
    SetRotation({ D3DXToRadian(-90.f + angle), D3DXToRadian(90.f), 0.f }, "RFoot3");
    
    if (StandToCrawlAnim.ElapsedTime > StandToCrawlAnim.TotalTime)
    {
        Crawl();
    }
}

void Ender::PlayStand(_float dt)
{
    StandAnim.ElapsedTime += dt;

    WalkTime += dt;
    if (WalkTime > 0.3f)
    {
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkEnder");
        WalkTime = 0.f;
    }

    float LegSwingAngle = D3DXToRadian(30.f);
    float AnimSpeed = 6.f;

    float phaseLLeg1 = 0.f;
    float phaseRLeg2 = D3DXToRadian(120.f);
    float phaseLLeg3 = D3DXToRadian(240.f);

    float phaseRLeg1 = D3DXToRadian(180.f);
    float phaseLLeg2 = D3DXToRadian(300.f);
    float phaseRLeg3 = D3DXToRadian(60.f);

    float angleLLeg1 = sinf(StandAnim.ElapsedTime * AnimSpeed + phaseLLeg1);
    float angleRLeg2 = sinf(StandAnim.ElapsedTime * AnimSpeed + phaseRLeg2);
    float angleLLeg3 = sinf(StandAnim.ElapsedTime * AnimSpeed + phaseLLeg3);

    float angleRLeg1 = sinf(StandAnim.ElapsedTime * AnimSpeed + phaseRLeg1);
    float angleLLeg2 = sinf(StandAnim.ElapsedTime * AnimSpeed + phaseLLeg2);
    float angleRLeg3 = sinf(StandAnim.ElapsedTime * AnimSpeed + phaseRLeg3);

    SetRotation({ LegSwingAngle * angleLLeg1, 0.f, 0.f }, "LLeg1");
    SetRotation({ LegSwingAngle * angleRLeg2, 0.f, 0.f }, "RLeg2");
    SetRotation({ LegSwingAngle * angleLLeg3, 0.f, 0.f }, "LLeg3");

    SetRotation({ LegSwingAngle * angleRLeg1, 0.f, 0.f }, "RLeg1");
    SetRotation({ LegSwingAngle * angleLLeg2, 0.f, 0.f }, "LLeg2");
    SetRotation({ LegSwingAngle * angleRLeg3, 0.f, 0.f }, "RLeg3");
}

_float Ender::EasedT(_float t)
{
    if (t == 0.0f) return 0.0f;
    if (t == 1.0f) return 1.0f;
    float p = 0.3f;
    float s = p / 4.0f;
    float u = t - 1.0f;
    return -std::pow(2.0f, 10.0f * u) * std::sin((u - s) * (2.0f * 3.14) / p);
}

void Ender::PlayHide(_float dt)
{
    HideAnim.ElapsedTime += dt;

    _float t = clamp(HideAnim.ElapsedTime / HideAnim.TotalTime, 0.f, 1.f);
    
    _float eased_t = EasedT(t);

    _float size = lerp(0.1f, 0.01f, eased_t);

    _float yOffset = lerp(HideStartY, HideStartY - 13.f, eased_t);

    auto transform = Bones["Body"]->GetComponent<TransformComponent>();
    _vec3 pos = transform->GetPosition();
    
    transform->SetPosition(_vec3(pos.x, yOffset, pos.z));

    Scale = size;
    SetBoneSize();
   
   if (HideAnim.ElapsedTime > HideAnim.TotalTime)
   {
       for (auto& Bone : Bones)
       {
           if (Bone.second == nullptr) continue;
           auto renderer = Bone.second->GetComponent<MeshRenderer>();
           renderer->SetRenderID(RENDER_ID::Render_None);
       }
       *IsDie = false;
       HideIdle();
   }
}

void Ender::PlayHideIdle(_float dt)
{
    WalkTime += dt;
    if (WalkTime > 0.3f)
    {
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkEnder");
        WalkTime = 0.f;
    }
}

void Ender::PlaySprout(_float dt)
{
    SproutAnim.ElapsedTime += dt;

    _float t = clamp(SproutAnim.ElapsedTime / SproutAnim.TotalTime, 0.f, 1.f);

    _float eased_t = EasedT(t);

    _float size = lerp(0.01f, 0.1f, eased_t);

    _float yOffset = lerp(HideStartY - 13.f, HideStartY, eased_t);

    auto transform = Bones["Body"]->GetComponent<TransformComponent>();
    _vec3 pos = transform->GetPosition();

    transform->SetPosition(_vec3(pos.x, yOffset, pos.z));

    if (t > 0.5f && !IsSproutPlay)
    {
        IsSproutPlay = true;
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("SproutEnder");
    }

    Scale = size;
    SetBoneSize();

    if (SproutAnim.ElapsedTime > SproutAnim.TotalTime)
    {
        Stand();
    }
}

void Ender::PlayLineLaserAttack(_float dt)
{
    HeadAttackAnim.DelayTime -= dt;
    LaserSpawnTime += dt;

    if (LaserSpawnTime > 0.5f)
    {
        LaserSpawnTime = 0.f;

        auto Transform = GetComponent<TransformComponent>();
        _vec3 Pos = Transform->GetPosition();

        _float RandX = rand() % 40 - 20;
        _float RandZ = rand() % 40 - 20;

        _vec3 randPos = _vec3(Pos.x + RandX, Pos.y + 50.f, Pos.z + RandZ);
        auto laserhead = LaserHead::Create(owner, ObjectType::Bone);
        auto projectileTransform = laserhead->GetComponent<TransformComponent>();
        projectileTransform->SetPosition(randPos);

        laserhead->SetDir(HeadDir(rand() % 4));
        laserhead->SetActive(true);
    }

    if (HeadAttackAnim.DelayTime < 0)
    {
        //HeadAttackAnim.IsEnd = true;

        (*IsAttack) = false;
        AttackAnim.IsEnd = true;
        CurChangeStateCount = 0;
        Sprout();
        *ChangeStateCount = rand() % 3 + 1;
    }
}

void Ender::PlayCrossLaserAttack(_float dt)
{
    LaserAttackAnim.ElapsedTime += dt;
    CrossLaserSoundTime -= dt;
    if (CrossLaserSoundTime < 0.f)
    {
        CrossLaserSoundTime = 1.f;
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("CrossLaserEnder");

    }
    for (auto& CrossLaser : CrossLasers)
    {
        auto Lasertransform = CrossLaser->GetComponent<TransformComponent>();
        _vec3 rot = Lasertransform->GetRotate();
        Lasertransform->SetRotate(_vec3(rot.x, rot.y + 1.f * dt, rot.z));
    }

    if (LaserAttackAnim.ElapsedTime > LaserAttackAnim.TotalTime)
    {
        for (auto& CrossLaser : CrossLasers)
            CrossLaser->SetActive(false);

        (*IsAttack) = false;
        AttackAnim.IsEnd = true;
        CurChangeStateCount = 0;
        Stand();
        *ChangeStateCount = rand() % 3 + 1;
    }
}

void Ender::PlayProjectileAttack(_float dt)
{
    ProjectileAttackAnim.DelayTime -= dt;
    ProjectileSpawnTime += dt;
    ProjectileAttackAnim.ElapsedTime += dt;

    if (ProjectileAttackAnim.Phase == Ready)
    {
        _float t = clamp(ProjectileAttackAnim.ElapsedTime / ProjectileAttackAnim.TotalTime, 0.f, 1.f);
        _float angle = lerp(ProjectileAttackAnim.Start, ProjectileAttackAnim.End, t);
        auto toptransform = Bones["TopHead"]->GetComponent<TransformComponent>();
        toptransform->SetRotate(D3DXToRadian(angle), 0.f, 0.f);

        if (ProjectileAttackAnim.TotalTime < ProjectileAttackAnim.ElapsedTime)
        {
            ProjectileAttackAnim.Phase = Action;
            ProjectileAttackAnim.ElapsedTime = 0.f;
            ProjectileAttackAnim.TotalTime = 3.f;

            EngineCore::GetInstance()->GetSoundManager()->PlaySFX("ProjectileEnder");
        }
    }
    if (ProjectileAttackAnim.Phase == Action)
    {
        if (ProjectileSpawnTime > 0.5f)
        {
            auto Transform = GetComponent<TransformComponent>();
            _vec3 Pos = Transform->GetPosition();

            ProjectileSpawnTime = 0.f;
            auto projectileTransform = EnderProjectiles[ProjectileIndex]->GetComponent<TransformComponent>();
            projectileTransform->SetPosition(Pos.x, Pos.y + 60.f * Scale, Pos.z);

            auto player = owner->GetFrontObject(ObjectType::Player);
            _vec3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();
            int randx = rand() % 1 - 0.5f;
            int randz = rand() % 1 - 0.5f;
            _vec3 randPos = _vec3(randx, 1.f, randz);

            _vec3 targetPos = playerPos + randPos - Pos;
            _float length = D3DXVec3Length(&targetPos);
            D3DXVec3Normalize(&targetPos, &targetPos);

            int randPower = rand() % 10 - 10 + length;

            static_cast<EnderProjectile*>(EnderProjectiles[ProjectileIndex])->FireProjectile(targetPos * randPower);
            static_cast<EnderProjectile*>(EnderProjectiles[ProjectileIndex++])->SetVisible(true);

            if (ProjectileIndex >= 30) ProjectileIndex = 0;
        }
        if (ProjectileAttackAnim.TotalTime < ProjectileAttackAnim.ElapsedTime)
        {
            ProjectileAttackAnim.Phase = Recover;
            ProjectileAttackAnim.ElapsedTime = 0.f;
            ProjectileAttackAnim.TotalTime = 1.f;
        }
    }

    if (ProjectileAttackAnim.Phase == Recover)
    {
        _float t = clamp(ProjectileAttackAnim.ElapsedTime / ProjectileAttackAnim.TotalTime, 0.f, 1.f);
        _float angle = lerp(ProjectileAttackAnim.End, ProjectileAttackAnim.Start, t);
        auto toptransform = Bones["TopHead"]->GetComponent<TransformComponent>();
        toptransform->SetRotate(D3DXToRadian(angle), 0.f, 0.f);

        if (ProjectileAttackAnim.TotalTime < ProjectileAttackAnim.ElapsedTime)
        {
            ProjectileAttackAnim.IsEnd = true;

            (*IsAttack) = false;
            AttackAnim.IsEnd = true;
            CurChangeStateCount = 0;
            Stand();

            ProjectileAttackAnim.Phase = Ready;
            ProjectileAttackAnim.ElapsedTime = 0.f;
            ProjectileAttackAnim.TotalTime = 1.f;

            *ChangeStateCount = rand() % 3 + 1;
        }
    }
}

void Ender::PlayDie(_float dt)
{
    DieAnim.ElapsedTime += dt;

    float t = clamp(DieAnim.ElapsedTime / DieAnim.TotalTime, 0.f, 1.f);

    float val = lerp(1.f, 0.f, t);

    for (auto& material : materials)
    {
        material->SetFloat("alpha", val);
    }

    if (!deadEffect)
    {
        auto effect = EnderDead::Create(owner, ObjectType::ParticleEffect);
        effect->SetDeadTime(7.5f);
        effect->GetComponent<TransformComponent>()->SetPosition(GetComponent<TransformComponent>()->GetPosition());
        owner->AddObject(ObjectType::ParticleEffect, effect);

        deadEffect = true;
    }


    if (DieAnim.ElapsedTime > DieAnim.TotalTime)
    {
        auto effect = DeadEffect::Create(owner, ObjectType::ParticleEffect);
        effect->SetDeadTime(3.f);
        effect->GetComponent<TransformComponent>()->SetPosition(GetComponent<TransformComponent>()->GetPosition());
        owner->AddObject(ObjectType::ParticleEffect, effect);

        SetDead();
        DeleteBar();
    }
}

void Ender::SetBoneSize()
{
    auto toptransform = Bones["TopHead"]->GetComponent<TransformComponent>();
    toptransform->SetScale(24.f * Scale, 18.f * Scale, 24.f * Scale);
    toptransform->SetPosition(0.0f, 24.f * Scale, 0.f);
    toptransform->SetPivot(0.f, -18.f * Scale, 24.f * Scale);

    auto bottomtransform = Bones["Head"]->GetComponent<TransformComponent>();
    bottomtransform->SetScale(24.f * Scale, 6.f * Scale, 24.f * Scale);
    bottomtransform->SetPosition(0.f * Scale, 66.f * Scale, 0.f * Scale);
    bottomtransform->SetPivot(0.f, -84.f * Scale, 0.f);

    auto bodytransform = Bones["Body"]->GetComponent<TransformComponent>();
    bodytransform->SetScale(16.f * Scale, 60.f * Scale, 16.f * Scale);

    auto leg1transrorm = Bones["LLeg1"]->GetComponent<TransformComponent>();
    leg1transrorm->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    leg1transrorm->SetPosition(40.f * Scale, 45.f * Scale, 0 * Scale);
    leg1transrorm->SetPivot(-24.f * Scale, -45.f * Scale, 0.f);

    auto leg2transform = Bones["LLeg2"]->GetComponent<TransformComponent>();
    leg2transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    leg2transform->SetPosition(40.f * Scale, 0 * Scale, 0 * Scale);
    leg2transform->SetPivot(-24.f * Scale, 0.f, 0.f);

    auto leg3transform = Bones["LLeg3"]->GetComponent<TransformComponent>();
    leg3transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    leg3transform->SetPosition(40.f * Scale, -45.f * Scale, 0 * Scale);
    leg3transform->SetPivot(-24.f * Scale, 45.f * Scale, 0.f);

    auto foot1transform = Bones["LFoot1"]->GetComponent<TransformComponent>();
    foot1transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    foot1transform->SetPosition(54.f * Scale, 0.f, 0 * Scale);
    foot1transform->SetPivot(-30.f * Scale, 0.f, 0.f);

    auto foot2transform = Bones["LFoot2"]->GetComponent<TransformComponent>();
    foot2transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    foot2transform->SetPosition(54.f * Scale, 0.f, 0 * Scale);
    foot2transform->SetPivot(-30.f * Scale, 0.f, 0.f);

    auto foot3transform = Bones["LFoot3"]->GetComponent<TransformComponent>();
    foot3transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    foot3transform->SetPosition(54.f * Scale, 0.f, 0 * Scale);
    foot3transform->SetPivot(-30.f * Scale, 0.f, 0.f);

    auto rleg1transrorm = Bones["RLeg1"]->GetComponent<TransformComponent>();
    rleg1transrorm->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    rleg1transrorm->SetPosition(-40.f * Scale, 45.f * Scale, 0 * Scale);
    rleg1transrorm->SetPivot(24.f * Scale, -45.f * Scale, 0.f);

    auto rleg2transform = Bones["RLeg2"]->GetComponent<TransformComponent>();
    rleg2transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    rleg2transform->SetPosition(-40.f * Scale, 0 * Scale, 0 * Scale);
    rleg2transform->SetPivot(24.f * Scale, 0.f, 0.f);

    auto rleg3transform = Bones["RLeg3"]->GetComponent<TransformComponent>();
    rleg3transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    rleg3transform->SetPosition(-40.f * Scale, -45.f * Scale, 0 * Scale);
    rleg3transform->SetPivot(24.f * Scale, 45.f * Scale, 0.f);

    auto rfoot1transform = Bones["RFoot1"]->GetComponent<TransformComponent>();
    rfoot1transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    rfoot1transform->SetPosition(-54.f * Scale, 0.f, 0 * Scale);
    rfoot1transform->SetPivot(30.f * Scale, 0.f, 0.f);

    auto rfoot2transform = Bones["RFoot2"]->GetComponent<TransformComponent>();
    rfoot2transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    rfoot2transform->SetPosition(-54.f * Scale, 0.f, 0 * Scale);
    rfoot2transform->SetPivot(30.f * Scale, 0.f, 0.f);

    auto rfoot3transform = Bones["RFoot3"]->GetComponent<TransformComponent>();
    rfoot3transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    rfoot3transform->SetPosition(-54.f * Scale, 0.f, 0 * Scale);
    rfoot3transform->SetPivot(30.f * Scale, 0.f, 0.f);
}
