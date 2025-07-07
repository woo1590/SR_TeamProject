#include "pch.h"
#include "RedGolem.h"
#include "TransformComponent.h"
#include "InfoComponent.h"
#include "CollisionComponent.h"
#include "IsTargetInAttackRange.h"
#include "Chase.h"
#include "Attack.h"
#include "SequenceNode.h"
#include "SelectorNode.h"
#include "ObjectManager.h"
#include "BehaviorTree.h"
#include "Player.h"
#include "AIController.h"
#include "IsAlive.h"
#include "GolemProjectile.h"
#include "Die.h"

RedGolem::RedGolem(ObjectManager* owner, ObjectType objType)
	:Boss(owner, objType)
{
}

RedGolem::~RedGolem()
{
}

RedGolem* RedGolem::Create(ObjectManager* owner, ObjectType objType)
{
    RedGolem* Instance = new RedGolem(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT RedGolem::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Boss::Ready_Object(owner, objType);
    
    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(10.f, 18.f, 10.f));
    InitTransform(objType);
    InitTree();
    InitAnimation();

    InitProjectile(objType);

	return S_OK;
}

void RedGolem::Update(_float dt)
{
    Monster::Update(dt);
    PlayAnimation(dt);
}

void RedGolem::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void RedGolem::MoveTo(_vec3* dir, _float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    auto stat = GetComponent<InfoComponent<EnemyInfo>>();
    if (State != MonsterState::Walk) State = MonsterState::Walk;
    D3DXVec3Normalize(dir, dir);
    Transform->Translate(*dir * dt * stat->GetInfo().speed * 0.5f);
    Transform->SetForward(_vec3(dir->x, 0.f, dir->z));
}

void RedGolem::RotateTo(_vec3* dir, float dt)
{
}

void RedGolem::Attack(Object* target)
{
    if (State != MonsterState::Attack)
    {
        State = MonsterState::Attack;
        SuperAttackAnim.DelayTime = 0.f;
        LeftAttackAnim.DelayTime = 0.f;
        AttackAnim.ElapsedTime = 0;
    }
}

void RedGolem::Die()
{
}

void RedGolem::Hit(_vec3 dir, _float power)
{
}

void RedGolem::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);
    transform->SetPosition(20.f, 100.f, 10.f);
    //
    transform->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
    //

    SetMaterial(L"RedGolemBody_Mtrl", "Body");
    SetMaterial(L"RedGolemFace_Mtrl", "Head");
    SetMaterial(L"RedGolemArm_Mtrl", "LArm");
    SetMaterial(L"RedGolemArm_Mtrl", "RArm");
    SetMaterial(L"RedGolemFoot_Mtrl", "LLeg");
    SetMaterial(L"RedGolemFoot_Mtrl", "RLeg");

    //head
    SetScale(_vec3(15.f * Scale, 15.f * Scale, 10.f * Scale), "Head");
    SetPosition(_vec3(0.f, 17.5f * Scale, 27.5f * Scale), "Head");

    //body
    SetScale(_vec3(40.f * Scale, 30.f * Scale, 20.f * Scale), "Body");
    Add_Bone("LightBlock", objType, Bones["Body"], L"RedgolemLightblock_Mtrl");
    SetScale(_vec3(20.f * Scale, 20.f * Scale, 20.f * Scale), "LightBlock");
    SetPosition(_vec3(0.f, -3.f * Scale, -10.f * Scale), "LightBlock");
    Bones["Body"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.f, -20.f * Scale, 0.0f));
    Bones["Body"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    //arm
    SetScale(_vec3(10.f * Scale, 20.f * Scale, 10.f * Scale), "LArm");
    SetPosition(_vec3(-50.f * Scale, 12.f * Scale, 0.f), "LArm");
    Bones["LArm"]->GetComponent<TransformComponent>()->SetPivot(_vec3(5.f * Scale, -12.f * Scale, 0.0f));
    SetRotation(_vec3(D3DXToRadian(30.f), 0.f, 0.f), "LArm");

    SetScale(_vec3(10.f * Scale, 20.f * Scale, 10.f * Scale), "RArm");
    SetPosition(_vec3(50.f * Scale, 12.f * Scale, 0.f), "RArm");
    Bones["RArm"]->GetComponent<TransformComponent>()->SetPivot(_vec3(-5.f * Scale, -12.f * Scale, 0.0f));
    SetRotation(_vec3(D3DXToRadian(30.f), 0.f, 0.f), "RArm");

    //leg
    SetScale(_vec3(15.f * Scale, 25.f * Scale, 15.f * Scale), "LLeg");
    SetPosition(_vec3(-30.f * Scale, -52.5f * Scale, 0.f), "LLeg");
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 12.5f * Scale, 0.0f));

    SetScale(_vec3(15.f * Scale, 25.f * Scale, 15.f * Scale), "RLeg");
    SetPosition(_vec3(30.f * Scale, -52.5f * Scale, 0.f), "RLeg");
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 12.5f * Scale, 0.0f));

    //hand
    Add_Bone("LHand", objType, Bones["LArm"], L"RedGolemHand_Mtrl");
    SetScale(_vec3(10.f * Scale, 20.f * Scale, 10.f * Scale), "LHand");
    SetPosition(_vec3(-5.f * Scale, -40.f * Scale, 0.f), "LHand");
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivot(_vec3(5.f* Scale, 20.f * Scale, 0.0f));
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    SetRotation(_vec3(D3DXToRadian(-35.f), 0.f, 0.f), "LHand");

    Add_Bone("RHand", objType, Bones["RArm"], L"RedGolemHand_Mtrl");
    SetScale(_vec3(10.f * Scale, 20.f * Scale, 10.f * Scale), "RHand");
    SetPosition(_vec3(5.f * Scale, -40.f * Scale, 0.f), "RHand");
    Bones["RHand"]->GetComponent<TransformComponent>()->SetPivot(_vec3(-5.f * Scale, 20.f * Scale, 0.0f));
    Bones["RHand"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    SetRotation(_vec3(D3DXToRadian(-35.f), 0.f, 0.f), "RHand");
}

void RedGolem::InitTree()
{
    //blackboard 
    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    bb->SetValue("Target", owner->GetObjectList(ObjectType::Player).back());
    Distance = new float(10.f);
    bb->SetValue("Distance", Distance);

    //BT
    IsTargetInAttackRange* attackCheck = new IsTargetInAttackRange(new AttackNode());

    SequenceNode* attackSequence = new SequenceNode();
    attackSequence->AddChild(attackCheck);

    SelectorNode* BehaviorNode = new SelectorNode();
    BehaviorNode->AddChild(attackSequence);
    BehaviorNode->AddChild(new ChaseNode());

    IsAliveNode* IsAlive = new IsAliveNode(BehaviorNode);

    SelectorNode* root = new SelectorNode();
    root->AddChild(IsAlive);
    root->AddChild(new DieNode());

    BehaviorTree* bt = BehaviorTree::Create(root);

    //AI
    auto AI = AddComponent<AIController>(bt, bb);
}

void RedGolem::InitAnimation()
{
    WalkAnim.ElapsedTime = 0.f;

    //LeftAttack
    LeftAttackAnim.ElapsedTime = 0.f;
    LeftAttackAnim.TotalTime = 1.8f;
    LeftAttackAnim.DelayTime = 0.f;

    //SuperAttack
    SuperAttackAnim.ElapsedTime = 0.f;
    SuperAttackAnim.TotalTime = 1.5f;
    SuperAttackAnim.DelayTime = 0.f;

    //Hit
    HitAnim.Start = 0;
    HitAnim.End = 30.f;
    HitAnim.ElapsedTime = 0.f;
    HitAnim.TotalTime = 0.3f;
    HitAnim.DelayTime = 0.f;

    //Die
    DieAnim.Start = 0;                  //start angle
    DieAnim.End = 270.f;                //end angle
    DieAnim.ElapsedTime = 0.f;
    DieAnim.TotalTime = 0.5f;          //play animation total time
}

void RedGolem::PlayAnimation(_float dt)
{
    switch (State)
    {
    case MonsterState::Idle:
        PlayIdle(dt);
        break;
    case MonsterState::Walk:
        PlayWalk(dt);
        break;
    case MonsterState::Attack:
        if (!AttackAnim.IsRunning) AttackAnim.IsRunning = true;
        PlayAttack(dt);
        break;
    case MonsterState::Hit:
        if (!HitAnim.IsRunning) HitAnim.IsRunning = true;
        PlayHit(dt);
        break;
    case MonsterState::Die:
        if (!DieAnim.IsRunning) DieAnim.IsRunning = true;
        PlayDie(dt);
        break;
    }
}

void RedGolem::PlayIdle(_float dt)
{
}

void RedGolem::PlayWalk(_float dt)
{
    WalkAnim.ElapsedTime += dt;
    
    float Angle = sinf(WalkAnim.ElapsedTime * Speed);

    float LegAngle = D3DXToRadian(20.f) * Angle;
    float ArmAngle = D3DXToRadian(30.f) * Angle;

    SetRotation({ LegAngle, 0.f, 0.f }, "LLeg");
    SetRotation({ -LegAngle, 0.f, 0.f }, "RLeg");

    SetRotation({ -ArmAngle, 0.f, 0.f }, "LArm");
    SetRotation({ ArmAngle, 0.f, 0.f }, "RArm");

    auto transform = GetComponent<TransformComponent>();
    _vec3 pos = transform->GetPosition();

    float bodyBob = Angle * 0.015f;
    transform->SetPosition(_vec3(pos.x, pos.y + bodyBob, pos.z));

    float bodyYaw = D3DXToRadian(2.f) * sinf(Angle * D3DX_PI * 2);
    SetRotation({ 0.f, bodyYaw, 0.f }, "Body");
}

void RedGolem::PlayAttack(_float dt)
{
    auto bb = GetComponent<AIController>()->GetBlackBoard();

   
    //int* number = static_cast<_int*>(bb->GetValue("AttackNumber"));
    switch (2)
    {
    case 1:                         //ÆÈ ÈÖµÎ¸£±â
        PlayLeftAttack(dt);
        break;
    case 2:                         //³»·ÁÂï±â
        PlaySuperAttack(dt);
        break;
    }
}

void RedGolem::PlayDie(_float dt)
{
}

void RedGolem::PlayHit(_float dt)
{
    //effect
    //motin xxx
}

void RedGolem::OnCollisionStay(Object* other)
{
}

void RedGolem::PlayLeftAttack(_float dt)
{
    LeftAttackAnim.DelayTime -= dt;
    if (LeftAttackAnim.DelayTime > 0.f)
        return;

    LeftAttackAnim.ElapsedTime += dt;

    float t = LeftAttackAnim.ElapsedTime / LeftAttackAnim.TotalTime;
    float angleCurve = sinf(t * D3DX_PI * 2);

    float bodyYaw = D3DXToRadian(30.f) * angleCurve;
    SetRotation(_vec3(0.f, bodyYaw, 0.f));

    float armX = D3DXToRadian(-60.f + 90.f * angleCurve); 
    float armZ = D3DXToRadian(-30.f + 45.f * angleCurve); 
    SetRotation(_vec3(armX, 0.f, armZ), "LArm");

    float handX = D3DXToRadian(-30.f + 75.f * angleCurve);
    SetRotation(_vec3(handX, 0.f, 0.f), "LHand");

    float Angle = sinf(WalkAnim.ElapsedTime * Speed);
    t = std::clamp(t, 0.f, 1.f); 

    float LegAngle = D3DXToRadian(20.f) * Angle;

    SetRotation({ LegAngle, 0.f, 0.f }, "LLeg");
    SetRotation({ -LegAngle, 0.f, 0.f }, "RLeg");

    if (LeftAttackAnim.ElapsedTime >= LeftAttackAnim.TotalTime)
    {
        LeftAttackAnim.ElapsedTime = 0.f;
        LeftAttackAnim.DelayTime = 1.5f;
        LeftAttackAnim.IsRunning = false;
        LeftAttackAnim.IsEnd = true;

        SetRotation(_vec3(D3DXToRadian(30.f), 0.f, 0.f), "LArm");
        SetRotation(_vec3(D3DXToRadian(-35.f), 0.f, 0.f), "LHand");
    }
}

void RedGolem::PlaySuperAttack(_float dt)
{
    if (SuperAttackAnim.DelayTime > 0.f)
    {
        PlaySuperAttackDelay(dt);
        return;
    }
    SuperAttackAnim.ElapsedTime += dt;

    _float t = SuperAttackAnim.ElapsedTime / SuperAttackAnim.TotalTime;
    t = std::clamp(t, 0.f, 1.f);
    _float charge = 0.5f; 

    auto transform = GetComponent<TransformComponent>();
    auto collision = GetComponent<CollisionComponent>();
    
    if (t < charge) 
    {
        _float c = t / charge;

        _float armBack = D3DXToRadian(-90.f);
        _float bodyLeanBack = D3DXToRadian(15.f);

        SetRotation({ armBack * c, 0.f, 0.f }, "LArm");
        SetRotation({ armBack * c, 0.f, 0.f }, "RArm");
        SetRotation({ -bodyLeanBack * c, 0.f, 0.f }, "Body"); 
        SetRotation({ bodyLeanBack * c, 0.f, 0.f }, "LLeg");
        SetRotation({ bodyLeanBack * c, 0.f, 0.f }, "RLeg"); 
    }
    else
    {
        _float s = (t - charge) / (1.f - charge);

        _float armSmash = D3DXToRadian(90.f);
        _float bodyLeanForward = D3DXToRadian(60.f);

        SetRotation({ bodyLeanForward * s, 0.f, 0.f }, "Body"); 
        SetRotation({ -bodyLeanForward * s, 0.f, 0.f }, "LLeg");
        SetRotation({ -bodyLeanForward * s, 0.f, 0.f }, "RLeg");
        _vec3 pos = transform->GetPosition();
        transform->SetPosition(_vec3(pos.x, pos.y - 15 * Scale * (1 - sin(bodyLeanForward * s)), pos.z));
        collision->SetSize(_vec3(10.f, 18.f - 15 * Scale * (1 - sin(bodyLeanForward * s)), 10.f));
    }
   
    if (SuperAttackAnim.ElapsedTime >= SuperAttackAnim.TotalTime)
    {
        SuperAttackAnim.IsRunning = false;
        SuperAttackAnim.ElapsedTime = 0.f;
        SuperAttackAnim.DelayTime = 2.f;
    }
}

void RedGolem::PlaySuperAttackDelay(_float dt)
{
    SuperAttackAnim.DelayTime -= dt;
    SpawnTime += dt;

    if (SpawnTime > 0.2f)
    {
        auto Transform = GetComponent<TransformComponent>();
        _vec3 Pos = Transform->GetPosition();

        _float RandX = rand() % 40 - 20;
        _float RandZ = rand() % 40 - 20;

        _vec3 randPos = _vec3(Pos.x + RandX,Pos.y -8.5f, Pos.z + RandZ);

        SpawnTime = 0.f;
        auto projectileTransform = BoxProjectile[Index]->GetComponent<TransformComponent>();
        projectileTransform->SetPosition(randPos);
        static_cast<GolemProjectile*>(BoxProjectile[Index++])->SetOn(true);

        if (Index >= 10) Index = 0;

    }

    if (SuperAttackAnim.DelayTime < 0)
    {
        SuperAttackAnim.IsEnd = true;
    }
}

void RedGolem::InitProjectile(ObjectType objType)
{
    BoxProjectile.reserve(10);

    for (int i = 0; i < 10; ++i)
    {
        BoxProjectile.push_back(GolemProjectile::Create(owner, objType));
    }
}

void RedGolem::Free()
{
    Monster::Free();
}
