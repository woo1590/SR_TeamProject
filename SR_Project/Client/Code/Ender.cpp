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

Ender::Ender(ObjectManager* owner, ObjectType objType)
	:Monster(owner, objType)
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
    Monster::Ready_Object(owner, objType);

    InitTransform(objType);
    InitTree();
    InitAnimation();

    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(10.f, 20.f, 20.f));

    Stand();

	return S_OK;
}

void Ender::Update(_float dt)
{
    Monster::Update(dt);
    PlayAnimation(dt);
}

void Ender::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void Ender::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    Add_Bone("TopHead", objType, Bones["Head"], "EnderTopHead_Mtrl");
    SetMaterial("EnderBottomHead_Mtrl", "Head", RENDER_ID::Render_NonAlpha);
    SetMaterial("EnderBody_Mtrl", "Body");

    //transform->SetRotate(_vec3(0.f, D3DXToRadian(90.f), 0.f/*D3DXToRadian(-90.f)*/));
    transform->SetPosition(10 , 100, 10);
    
    auto toptransform = Bones["TopHead"]->GetComponent<TransformComponent>();
    toptransform->SetScale(24.f * Scale, 18.f * Scale, 24.f * Scale);
    toptransform->SetPosition(0.0f, 24.f * Scale, 0.f);
    toptransform->SetRotate(D3DXToRadian(-45.f), 0.f, 0.f);
    toptransform->SetPivot(0.f, -18.f* Scale, 24.f * Scale);
    toptransform->SetPivotEnable(true);

    auto bottomtransform = Bones["Head"]->GetComponent<TransformComponent>();
    bottomtransform->SetScale(24.f * Scale, 6.f * Scale, 24.f * Scale);
    bottomtransform->SetPosition(0.f * Scale, 66.f * Scale, 0.f * Scale);
    bottomtransform->SetPivot(0.f, -84.f * Scale,0.f);
    bottomtransform->SetPivotEnable(true);
    bottomtransform->SetRotate(_vec3(/*D3DXToRadian(-90.f)*/0.f, 0.f, 0.f));

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
}

void Ender::InitTree()
{
    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    ChangeStateCount = new int(2);
    bb->SetValue("TriggerCount", ChangeStateCount);
    IsAttack = new _bool(false);
    bb->SetValue("IsAttack", IsAttack);

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
    root->AddChild(new DieNode());

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

    HideAnim.TotalTime = 3.f;
    HideAnim.ElapsedTime = 0.f;

    SproutAnim.TotalTime = 3.f;
    SproutAnim.ElapsedTime = 0.f;
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
    }
}

void Ender::Free()
{
    Monster::Free();
}

void Ender::Crawl()
{
    if (enderState != EnderState::Crawl)
    {
        enderState = EnderState::Crawl;


        auto transform = Bones["Body"]->GetComponent<TransformComponent>();
        transform->SetRotate(_vec3(D3DXToRadian(89.f), /*D3DXToRadian(89.f)*/0.f,0.f /*D3DXToRadian(-89.f)*/));

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
        transform->SetRotate(_vec3(0.f, /*D3DXToRadian(89.f)*/0.f, 0.f));

        auto bottomtransform = Bones["Head"]->GetComponent<TransformComponent>();
        bottomtransform->SetRotate(_vec3(0.f, 0.f, 0.f));

        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(90.f), 0.f }, "RFoot1");
        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(90.f), 0.f }, "RFoot2");
        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(90.f), 0.f }, "RFoot3");

        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(-90.f), 0.f }, "LFoot1");
        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(-90.f), 0.f }, "LFoot2");
        SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(-90.f), 0.f }, "LFoot3");
    }
}

void Ender::Hide()
{
    if (enderState != EnderState::Hidden)
    {
        auto transform = Bones["Body"]->GetComponent<TransformComponent>();
        HideStartY = transform->GetPosition().y;
        enderState = EnderState::Hidden;
    }
}

void Ender::Sprout()
{
    if (enderState != EnderState::Sprout)
    {
        auto transform = Bones["Body"]->GetComponent<TransformComponent>();
        enderState = EnderState::Sprout;

        for (auto& Bone : Bones)
        {
            if (Bone.second == nullptr) continue;
            auto renderer = Bone.second->GetComponent<MeshRenderer>();
            renderer->SetRenderID(RENDER_ID::Render_NonAlpha);
        }
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
}

void Ender::CrossLaserAttack()
{
}

void Ender::ProjectileAttack()
{
}

void Ender::MoveTo(_vec3 targetPos, _float dt)
{
    auto transform = GetComponent<TransformComponent>();
    _vec3 pos = transform->GetPosition();

    pos.y = 0;
    _vec3 dir = targetPos - pos;
    dir.y = 0;

    if (D3DXVec3Length(&dir) >= 0.1)
    {
        D3DXVec3Normalize(&dir, &dir);
        transform->Translate(dir * dt * Speed);
        transform->SetForward(dir);
        //_vec3 right = transform->GetRight();
        //_matrix rotMat;
        //D3DXMatrixRotationAxis(&rotMat, &right, D3DXToRadian(-90.f));
        //D3DXVec3TransformNormal(&dir, &dir, &rotMat);
        //transform->SetForward(dir);
    }
}

EnderState Ender::GetState()
{
    return enderState;
}

int Ender::GetCurChangeStateCount()
{
    return CurChangeStateCount;
}

void Ender::PlayCrawl(_float dt)
{
    CrawlAnim.ElapsedTime += dt;

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
    transform->SetRotate(_vec3(D3DXToRadian(90.f - angle), /*D3DXToRadian(89.f)*/0.f, 0.f));

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
    transform->SetRotate(_vec3(D3DXToRadian(angle), /*D3DXToRadian(89.f)*/0.f, 0.f));

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

    Scale = size;
    SetBoneSize();

    if (SproutAnim.ElapsedTime > SproutAnim.TotalTime)
    {
        Stand();
    }
}

void Ender::PlayLineLaserAttack(_float dt)
{
}

void Ender::PlayCrossLaserAttack(_float dt)
{
}

void Ender::PlayProjectileAttack(_float dt)
{
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
