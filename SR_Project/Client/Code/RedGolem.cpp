#include "pch.h"
#include "RedGolem.h"
#include "TransformComponent.h"
#include "InfoComponent.h"
#include "CollisionComponent.h"
#include "IsTimerOn.h"
#include "BossChase.h"
#include "Attack.h"
#include "SequenceNode.h"
#include "IsTargetInRange.h"
#include "SelectorNode.h"
#include "ObjectManager.h"
#include "BehaviorTree.h"
#include "Player.h"
#include "AIController.h"
#include "IsAlive.h"
#include "GolemProjectile.h"
#include "Die.h"
#include "EngineCore.h"
#include "SoundManager.h"
#include "PhysicsComponent.h"
#include "Scene.h"
#include "PhysicsSystem.h"

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

    {
        auto LArmCollision = Bones["LArm"]->AddComponent<CollisionComponent>();
        LArmCollision->AddCollider<OBBCollider>();
        LArmCollision->SetOffset(_vec3(10.f, -2.f, 0.f));
        LArmCollision->SetSize(_vec3(7.f, 20.f, 7.f));
        LArmCollision->SetCollisionEnter([this](Object* other) {this->OnCollisionEnter(other); });
        //
        LArmCollision->SetLayer(LAYER_ENEMY);
        LArmCollision->SetMask(LAYER_PLAYER);
        auto LArmPhysics = Bones["LArm"]->AddComponent<PhysicsComponent>();
        LArmPhysics->SetKinematic(true);
        GetScene()->GetPhysicsStstem()->RegisterBody(LArmPhysics);
    }
    
    {
        auto RArmCollision = Bones["RArm"]->AddComponent<CollisionComponent>();
        RArmCollision->AddCollider<OBBCollider>();
        RArmCollision->SetOffset(_vec3(-10.f, -2.f, 0.f));
        RArmCollision->SetSize(_vec3(7.f, 20.f, 7.f));
        RArmCollision->SetCollisionEnter([this](Object* other) {this->OnCollisionEnter(other); });
        RArmCollision->SetLayer(LAYER_ENEMY);
        RArmCollision->SetMask(LAYER_PLAYER);
        //���⵵

        auto RArmPhysics = Bones["RArm"]->AddComponent<PhysicsComponent>();
        RArmPhysics->SetKinematic(true);
        GetScene()->GetPhysicsStstem()->RegisterBody(RArmPhysics);
    }

    InitTransform(objType);
    InitTree();
    InitAnimation();

    InitProjectile(objType);

	return S_OK;
}

void RedGolem::Update(_float dt)
{
    *AttackTimer -= dt;
    PlayAnimation(dt);
    Monster::Update(dt);
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
    if (D3DXVec3Length(dir) >= *Distance)
    {
        D3DXVec3Normalize(dir, dir);
        Transform->Translate(*dir * dt * stat->GetInfo().speed * 2.5f);
    }
    D3DXVec3Normalize(dir, dir);
    Transform->SetForward(_vec3(dir->x, 0.f, dir->z));
}

void RedGolem::RotateTo(_vec3* dir, float dt)
{
}

void RedGolem::Attack(Object* target)
{
    if (State != MonsterState::Attack)
    {
        IsAttackDamage = false;
        State = MonsterState::Attack;
        SuperAttackAnim.DelayTime = 0.f;
        LeftAttackAnim.DelayTime = 0.f;
        RightAttackAnim.DelayTime = 0.f;

        AttackAnim.ElapsedTime = 0;
        AttackAnim.IsRunning = true;
        AttackAnim.IsEnd = false;

        if (*AttackNum == 1 || *AttackNum == 3)
        {
            EngineCore::GetInstance()->GetSoundManager()->PlaySFX("SwipeGolem");
        }
    }
}

void RedGolem::Die()
{
    if (State != MonsterState::Die)
    {
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("DeathGolem");
        State = MonsterState::Die;

        DieAnim.IsRunning = true;
        DieAnim.IsEnd = false;
        DieAnim.DelayTime = 0.0f;
        auto collision = GetComponent<CollisionComponent>();
        collision->SetSize(_vec3(0.1, 0.1, 0.1));

        auto physics = GetComponent<PhysicsComponent>();
        physics->SetGround(false);
    }
}

void RedGolem::Hit(_vec3 dir, _float power)
{
}

void RedGolem::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);
    transform->SetPosition(50.f, 100.f, 10.f);
    //
    transform->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
    //

    SetMaterial("RedGolemBody_Mtrl", "Body");
    SetMaterial("RedGolemFace_Mtrl", "Head");
    SetMaterial("RedGolemArm_Mtrl", "LArm");
    SetMaterial("RedGolemArm_Mtrl", "RArm");
    SetMaterial("RedGolemFoot_Mtrl", "LLeg");
    SetMaterial("RedGolemFoot_Mtrl", "RLeg");

    //head
    SetScale(_vec3(15.f * Scale, 15.f * Scale, 10.f * Scale), "Head");
    SetPosition(_vec3(0.f, 17.5f * Scale, 27.5f * Scale), "Head");

    //body
    SetScale(_vec3(40.f * Scale, 30.f * Scale, 20.f * Scale), "Body");
    Add_Bone("LightBlock", objType, Bones["Body"], "RedGolemLightblock_Mtrl");
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
    Add_Bone("LHand", objType, Bones["LArm"], "RedGolemHand_Mtrl");
    SetScale(_vec3(10.f * Scale, 20.f * Scale, 10.f * Scale), "LHand");
    SetPosition(_vec3(-5.f * Scale, -40.f * Scale, 0.f), "LHand");
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivot(_vec3(5.f* Scale, 20.f * Scale, 0.0f));
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    SetRotation(_vec3(D3DXToRadian(-35.f), 0.f, 0.f), "LHand");

    Add_Bone("RHand", objType, Bones["RArm"], "RedGolemHand_Mtrl");
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

    AttackTimer = new float(5.f);
    bb->SetValue("Timer", AttackTimer);

    AttackNum = new int(1);
    bb->SetValue("AttackNumber", AttackNum);

    //BT
    SelectorNode* attackSequence = new SelectorNode();
    attackSequence->AddChild(new IsTimerOn(new IsTargetInRange()));
    attackSequence->AddChild(new BossChase());

    SequenceNode* behaviorSelector = new SequenceNode();
    behaviorSelector->AddChild(attackSequence);          

    IsAliveNode* isAlive = new IsAliveNode(behaviorSelector);

    SelectorNode* root = new SelectorNode();
    root->AddChild(isAlive);
    root->AddChild(new DieNode());

    BehaviorTree* bt = BehaviorTree::Create(root);

    AddComponent<AIController>(bt, bb);
}

void RedGolem::InitAnimation()
{
    WalkAnim.ElapsedTime = 0.f;

    //LeftAttack
    LeftAttackAnim.ElapsedTime = 0.f;
    LeftAttackAnim.TotalTime = 1.f;
    LeftAttackAnim.DelayTime = 0.f;

    //RightAttack
    RightAttackAnim.ElapsedTime = 0.f;
    RightAttackAnim.TotalTime = 1.f;
    RightAttackAnim.DelayTime = 0.f;

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
    DieAnim.End = 90.f;                //end angle
    DieAnim.ElapsedTime = 0.f;
    DieAnim.TotalTime = 1.0f;          //play animation total time
    DieAnim.DelayTime = 1.5f;
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
    
    WalkTimer += dt;
    if (WalkTimer > 0.7f)
    {
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkGolem");
        WalkTimer = 0.f;
    }
    float Angle = sinf(WalkAnim.ElapsedTime * Speed);

    float LegAngle = D3DXToRadian(20.f) * Angle;
    float ArmAngle = D3DXToRadian(30.f) * Angle;

    SetRotation({ LegAngle/4, 0.f, 0.f }, "LLeg");
    SetRotation({ -LegAngle/4, 0.f, 0.f }, "RLeg");

    SetRotation({ -ArmAngle, 0.f, 0.f }, "LArm");
    SetRotation({ ArmAngle, 0.f, 0.f }, "RArm");

    auto transform = GetComponent<TransformComponent>();
    _vec3 pos = transform->GetPosition();

    float bodyYaw = D3DXToRadian(2.f) * sinf(Angle * D3DX_PI * 2);
    SetRotation({ 0.f, bodyYaw, 0.f }, "Body");
}

void RedGolem::PlayAttack(_float dt)
{
    switch (*AttackNum)
    {
    case 1:         
        PlayLeftAttack(dt);
        break;
    case 2:                         //�������
        PlaySuperAttack(dt);
        break;
    case 3:
        PlayRightAttack(dt);
        break;
    }
}

void RedGolem::PlayDie(_float dt)
{
    DieAnim.ElapsedTime += dt;

    float t = clamp(DieAnim.ElapsedTime / DieAnim.TotalTime, 0.f, 1.f);
    float angle = lerp(DieAnim.Start, DieAnim.End, t);

    SetRotation({ -D3DXToRadian(angle), 0.f, 0.f }, "Body");

    if (DieAnim.ElapsedTime > DieAnim.TotalTime)
    {
        DieAnim.IsEnd = true;

        SetRotation(_vec3(0.f, 0.f, D3DXToRadian(-20.f)), "LArm");
        SetRotation(_vec3(0.f, 0.f, D3DXToRadian(20.f)), "RArm");
      
        SetRotation(_vec3(D3DXToRadian(0.f), 0.f, 0.f), "LLeg");
        SetRotation(_vec3(D3DXToRadian(0.f), 0.f, 0.f), "RLeg");

        if (DieAnim.ElapsedTime > DieAnim.TotalTime + DieAnim.DelayTime)     //1.5ms -> delete
        {
            SetDead();
            DeleteBar();
        }
    }
}

void RedGolem::PlayHit(_float dt)
{
    //effect
    //motin xxx
}

void RedGolem::OnCollisionEnter(Object* other)
{
    ObjectType objType = other->GetObjectType();

    if (objType == ObjectType::Player)
    {
        if ((*AttackNum == 1 || *AttackNum == 3) && AttackAnim.IsRunning && !IsAttackDamage)
        {
            auto playerStat = other->GetComponent<InfoComponent<PlayerInfo>>();
            playerStat->AddHp(-40.f);
            IsAttackDamage = true;
        }
    }
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

        AttackAnim.IsRunning = false;
        AttackAnim.IsEnd = true;
        (*AttackTimer) = 5;
        SetRotation(_vec3(D3DXToRadian(30.f), 0.f, 0.f), "LArm");
        SetRotation(_vec3(D3DXToRadian(-35.f), 0.f, 0.f), "LHand");
    }
}

void RedGolem::PlayRightAttack(_float dt)
{
    RightAttackAnim.DelayTime -= dt;
    if (RightAttackAnim.DelayTime > 0.f)
        return;

    RightAttackAnim.ElapsedTime += dt;

    float t = RightAttackAnim.ElapsedTime / RightAttackAnim.TotalTime;
    float angleCurve = sinf(t * D3DX_PI * 2);

    float bodyYaw = D3DXToRadian(-30.f) * angleCurve; 
    SetRotation(_vec3(0.f, bodyYaw, 0.f));

    float armX = D3DXToRadian(-60.f + 90.f * angleCurve);
    float armZ = D3DXToRadian(30.f - 45.f * angleCurve); 
    SetRotation(_vec3(armX, 0.f, armZ), "RArm");

    float handX = D3DXToRadian(-30.f + 75.f * angleCurve);
    SetRotation(_vec3(handX, 0.f, 0.f), "RHand");

    float Angle = sinf(WalkAnim.ElapsedTime * Speed);
    t = std::clamp(t, 0.f, 1.f);

    float LegAngle = D3DXToRadian(20.f) * Angle;

    SetRotation({ LegAngle, 0.f, 0.f }, "LLeg");
    SetRotation({ -LegAngle, 0.f, 0.f }, "RLeg");

    if (RightAttackAnim.ElapsedTime >= RightAttackAnim.TotalTime)
    {
        RightAttackAnim.ElapsedTime = 0.f;
        RightAttackAnim.DelayTime = 1.5f;
        RightAttackAnim.IsRunning = false;
        RightAttackAnim.IsEnd = true;

        AttackAnim.IsRunning = false;
        AttackAnim.IsEnd = true;
        (*AttackTimer) = 5;
        SetRotation(_vec3(D3DXToRadian(30.f), 0.f, 0.f), "RArm");
        SetRotation(_vec3(D3DXToRadian(-35.f), 0.f, 0.f), "RHand");
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

        float colSize = lerp(18.f, 15.f, s);

        _float armSmash = D3DXToRadian(90.f);
        _float bodyLeanForward = D3DXToRadian(60.f);

        SetRotation({ bodyLeanForward * s, 0.f, 0.f }, "Body"); 
        SetRotation({ -bodyLeanForward * s, 0.f, 0.f }, "LLeg");
        SetRotation({ -bodyLeanForward * s, 0.f, 0.f }, "RLeg");
        _vec3 pos = transform->GetPosition();
        transform->SetPosition(_vec3(pos.x, pos.y - 15 * Scale * (1 - sin(bodyLeanForward * s)), pos.z));
        collision->SetSize(_vec3(10.f, colSize, 10.f));
    
        auto physics = GetComponent<PhysicsComponent>();
        physics->SetGround(false);
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

    if (SpawnTime > 0.3f)
    {
        auto Transform = GetComponent<TransformComponent>();
        _vec3 Pos = Transform->GetPosition();

        _float RandX = rand() % 40 - 20;
        _float RandZ = rand() % 40 - 20;

        _vec3 randPos = _vec3(Pos.x + RandX, Pos.y -6.f, Pos.z + RandZ);

        SpawnTime = 0.f;
        auto projectileTransform = BoxProjectile[Index]->GetComponent<TransformComponent>();
        projectileTransform->SetPosition(randPos);
        static_cast<GolemProjectile*>(BoxProjectile[Index++])->SetOn(true);

        if (Index >= 10) Index = 0;

    }

    if (SuperAttackAnim.DelayTime < 0)
    {
        SuperAttackAnim.IsEnd = true;
        AttackAnim.IsRunning = false;
        AttackAnim.IsEnd = true;
        (*AttackTimer) = 5;
        SetRotation(_vec3(D3DXToRadian(30.f), 0.f, 0.f), "RArm");
        SetRotation(_vec3(D3DXToRadian(-35.f), 0.f, 0.f), "RHand");

        SetRotation(_vec3(D3DXToRadian(30.f), 0.f, 0.f), "LArm");
        SetRotation(_vec3(D3DXToRadian(-35.f), 0.f, 0.f), "LHand");
        auto collision = GetComponent<CollisionComponent>();
        collision->SetSize(_vec3(10.f, 18.f, 10.f));
    }
}

void RedGolem::InitProjectile(ObjectType objType)
{
    BoxProjectile.reserve(10);

    for (int i = 0; i < 10; ++i)
    {
        BoxProjectile.push_back(GolemProjectile::Create(owner, ObjectType::Projectile));
    }
}

void RedGolem::Free()
{
    Safe_Delete(AttackTimer);
    Safe_Delete(AttackNum);
    Monster::Free();
}
