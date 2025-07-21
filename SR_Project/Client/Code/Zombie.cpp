#include "pch.h"
#include "Zombie.h"
#include "IsTargetInAttackRange.h"
#include "Chase.h"
#include "Attack.h"
#include "SequenceNode.h"
#include "SelectorNode.h"
#include "ObjectManager.h"
#include "BehaviorTree.h"
#include "BlackBoard.h"
#include "InfoComponent.h"
#include "BloodEffect.h"
#include "Player.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "AIController.h"
#include "IsAlive.h"
#include "Die.h"
#include "EngineCore.h"
#include "PhysicsComponent.h"
#include "SoundManager.h"
#include "DeadEffect.h"

Zombie::Zombie(ObjectManager* owner, ObjectType objType)
    :Monster(owner, objType)
{
}

Zombie::~Zombie()
{
}

Zombie* Zombie::Create(ObjectManager* owner, ObjectType objType)
{
    Zombie* Instance = new Zombie(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Zombie::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Monster::Ready_Object(owner, objType);

    InitTransform(objType);
    InitTree();
    InitAnimation();
    return S_OK;
}

void Zombie::Update(_float dt)
{
    Monster::Update(dt);
    PlayAnimation(dt);
}

void Zombie::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void Zombie::MoveTo(_vec3* dir, _float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    auto stat = GetComponent<InfoComponent<EnemyInfo>>();
    if (State != MonsterState::Walk) State = MonsterState::Walk;
    D3DXVec3Normalize(dir, dir);
    Transform->Translate(*dir * dt * stat->GetInfo().speed);
    Transform->SetForward(_vec3(dir->x, 0.f, dir->z));
}

void Zombie::RotateTo(_vec3* dir, float dt)
{
}

void Zombie::Attack(Object* target)
{
    if (State != MonsterState::Attack)
    {
        State = MonsterState::Attack;
        AttackAnim.DelayTime = 0.f;
        AttackAnim.ElapsedTime = 0;
        SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
        SetRotation({ 0.f, 0.f, 0.f }, "RLeg");
        *IsAttack = true;
        IsAttackDamage = false;
    }
}

void Zombie::Die()
{
    if (State != MonsterState::Die)
    {
        State = MonsterState::Die;

        DieAnim.IsRunning = true;
        DieAnim.IsEnd = false;
        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "LArm");
        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "RArm");
        SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
        SetRotation({ 0.f, 0.f, 0.f }, "RLeg");
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("DeathZombie");
    }
}

void Zombie::Hit(_vec3 dir, _float power)
{
    if(State != MonsterState::Hit)
    {
        State = MonsterState::Hit;

        HitAnim.IsRunning = true;
        HitAnim.IsEnd = false;
        HitAnim.ElapsedTime = 0.f;
        HitAnim.DelayTime = 0.f;
        HitAnim.Phase = Action;

        HitPrevRotHead = Bones["Head"]->GetComponent<TransformComponent>()->GetRotate();
        HitPrevRotLArm = Bones["Head"]->GetComponent<TransformComponent>()->GetRotate();
        HitPrevRotRArm = Bones["Head"]->GetComponent<TransformComponent>()->GetRotate();
        
        HitDir = dir;
        HitPower = power;

        ////////////////////////////////////////////////Blood Effect
        auto effect = BloodEffect::Create(owner, ObjectType::ParticleEffect);
        effect->SetDeadTime(1.f);
        effect->GetComponent<TransformComponent>()->SetPosition(GetComponent<TransformComponent>()->GetPosition());
        owner->AddObject(ObjectType::ParticleEffect, effect);
        ////////////////////////////////////////////////
        if (rand() % 2)
            EngineCore::GetInstance()->GetSoundManager()->PlaySFX("HitZombie");
        else
            EngineCore::GetInstance()->GetSoundManager()->PlaySFX("AttackZombie");

        Monster::Hit(dir, power);
    }
}

void Zombie::InitTransform(ObjectType objType)
{
    Bones["Body"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 6.f * Scale, 0.0f));
    Bones["Body"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    Bones["Head"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 8.f * Scale, 0.f));
    Bones["Head"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    auto transform = AddComponent<TransformComponent>();

    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(2.f, 7.f, 2.f));
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    {
        auto LArmCollision = Bones["LArm"]->AddComponent<CollisionComponent>();
        LArmCollision->AddCollider<OBBCollider>();
        LArmCollision->SetSize(_vec3(1.f, 4.f, 1.f));
        LArmCollision->SetCollisionEnter([this](Object* other) {this->OnCollisionStay(other);});
        LArmCollision->SetLayer(LAYER_ENEMY);
        LArmCollision->SetMask(LAYER_PLAYER);

        auto LArmPhysics = Bones["LArm"]->AddComponent<PhysicsComponent>();
        LArmPhysics->SetKinematic(true);
        GetScene()->GetPhysicsStstem()->RegisterBody(LArmPhysics);
    }

    transform->SetPosition(_vec3(15.f, 150.f, 50.f));
}

void Zombie::InitTree()
{
    //blackboard
    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    bb->SetValue("Target", owner->GetObjectList(ObjectType::Player).back());
    Distance = new float(3.f);
    bb->SetValue("Distance", Distance);
    IsHit = new _bool(false);
    bb->SetValue("IsDamaged", IsHit);
    IsAttack = new _bool(false);
    bb->SetValue("IsAttack", IsAttack);

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

void Zombie::InitAnimation()
{
    //Walk
    WalkAnim.ElapsedTime = 0.f;

    //Attack
    AttackAnim.ElapsedTime = 0.f;
    AttackAnim.TotalTime = 0.7f;
    AttackAnim.DelayTime = 0.f;

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

void Zombie::PlayAnimation(_float dt)
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
    if (State != MonsterState::Attack  && State != MonsterState::Die)
    {
        auto Transform = GetComponent<TransformComponent>();
        SetRotation(_vec3(Transform->GetRotate().x, 0.f, Transform->GetRotate().z), "Body");
    }   //modify
}

void Zombie::PlayIdle(_float dt)
{
    //idle animation
}

void Zombie::PlayWalk(_float dt)
{
    //walk animation
    WalkAnim.ElapsedTime += dt;

    float Angle = sinf(WalkAnim.ElapsedTime * Speed);

    SetRotation({ Angle, 0.f, 0.f }, "LLeg");
    SetRotation({ -Angle, 0.f, 0.f }, "RLeg");

    SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "LArm");
    SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "RArm");
}

void Zombie::PlayAttack(_float dt)
{
    //attack animation
    AttackAnim.DelayTime -= dt;
    if (AttackAnim.DelayTime > 0.f) return;

    AttackAnim.ElapsedTime += dt;

    float Angle = sinf(AttackAnim.ElapsedTime / AttackAnim.TotalTime * D3DX_PI * 2);

    auto Transform = GetComponent<TransformComponent>();
    //Body
    SetRotation({ -Angle / 2,0.f, 0.f });

    //Hand 
    SetRotation({ 180 - Angle * 2, 0.f, 0.f }, "LArm");
    SetRotation({ 180 - Angle * 2, 0.f, 0.f }, "RArm");

    // Leg
    SetRotation({ Angle / 2, 0.f, 0.f }, "LLeg");
    SetRotation({ Angle / 2, 0.f, 0.f }, "RLeg");

    if (AttackAnim.ElapsedTime > AttackAnim.TotalTime)
    {
        AttackAnim.IsRunning = false;
        AttackAnim.IsEnd = true;
        AttackAnim.ElapsedTime = 0.f;
        AttackAnim.DelayTime = 2.f;

        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "LArm");
        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "RArm");

        *IsAttack = false;
    }
}

void Zombie::PlayDie(_float dt)
{
    DieAnim.ElapsedTime += dt;

    _float t = clamp(DieAnim.ElapsedTime / DieAnim.TotalTime, 0.f, 1.f);

    _float Angle = lerp(DieAnim.Start, DieAnim.End, t);

    SetRotation({ 0.f, D3DXToRadian(Angle), -D3DXToRadian(Angle * 0.33f) });

    if (DieAnim.ElapsedTime > DieAnim.TotalTime)
    {
      //  DieAnim.ElapsedTime = 0.f;
        DieAnim.IsEnd = true;
        
        //////////////////////////////////Dead Effect
        auto effect = DeadEffect::Create(owner, ObjectType::ParticleEffect);
        effect->GetComponent<TransformComponent>()->SetPosition(GetComponent<TransformComponent>()->GetPosition());
        effect->SetDeadTime(1.5f);
        owner->AddObject(ObjectType::ParticleEffect, effect);
        //////////////////////////////////
        SetDead();
        DeleteBar();
    }
}

void Zombie::PlayHit(_float dt)
{
    HitAnim.ElapsedTime += dt;

    _float t = clamp(HitAnim.ElapsedTime / HitAnim.TotalTime, 0.f, 1.f);

    _float Angle = lerp(HitAnim.Start, HitAnim.End, t);
    float phaseProgress = clamp(HitAnim.ElapsedTime / HitAnim.TotalTime, 0.f, 1.f);

    switch (HitAnim.Phase)
    {
    case Phase::Action:
    {
        _float currentBendEffectAmountDeg = lerp(HitAnim.Start, HitAnim.End, phaseProgress); // 도(degree) 단위

        _vec3 targetHeadRotRad;
        targetHeadRotRad.x = HitPrevRotHead.x - D3DXToRadian(currentBendEffectAmountDeg / 3);
        targetHeadRotRad.y = HitPrevRotHead.y;
        targetHeadRotRad.z = HitPrevRotHead.z;
        SetRotation(targetHeadRotRad, "Head");

        _vec3 targetLArmRotRad;
        _vec3 targetRArmRotRad;

        _float targetLArmXDeg = D3DXToDegree(HitPrevRotLArm.x) - (currentBendEffectAmountDeg * 3);
        _float targetRArmXDeg = D3DXToDegree(HitPrevRotRArm.x) - (currentBendEffectAmountDeg * 3);

        targetLArmRotRad.x = D3DXToRadian(clamp(targetLArmXDeg, -180.f, D3DXToDegree(HitPrevRotLArm.x)));
        targetRArmRotRad.x = D3DXToRadian(clamp(targetRArmXDeg, -180.f, D3DXToDegree(HitPrevRotRArm.x)));

        targetLArmRotRad.y = HitPrevRotLArm.y;
        targetRArmRotRad.y = HitPrevRotRArm.y;

        targetLArmRotRad.z = HitPrevRotLArm.z - D3DXToRadian(currentBendEffectAmountDeg);
        targetRArmRotRad.z = HitPrevRotRArm.z - D3DXToRadian(currentBendEffectAmountDeg);

        SetRotation(targetLArmRotRad, "LArm");
        SetRotation(targetRArmRotRad, "RArm");

        PlayKnockBack(HitDir, HitPower, dt);

        if (HitAnim.ElapsedTime >= HitAnim.TotalTime)
        {
            HitAnim.Phase = Phase::Recover;
            HitAnim.ElapsedTime = 0.f;
            HitAnim.TotalTime = 0.3f;
        }
        break;
    }
    case Phase::Recover:
    {
        _vec3 currentRecoverHeadRot = Bones["Head"]->GetComponent<TransformComponent>()->GetRotate();
        _vec3 currentRecoverLArmRot = Bones["LArm"]->GetComponent<TransformComponent>()->GetRotate();
        _vec3 currentRecoverRArmRot = Bones["RArm"]->GetComponent<TransformComponent>()->GetRotate();
     
        _vec3 defaultArmRot = { D3DXToRadian(-90.f), 0.f, 0.f }; 

        _vec3 defaultOtherRot = { 0.f, 0.f, 0.f }; 
        SetRotation({ lerp(currentRecoverLArmRot.x, defaultArmRot.x, phaseProgress),
                      lerp(currentRecoverLArmRot.y, defaultArmRot.y, phaseProgress),
                      lerp(currentRecoverLArmRot.z, defaultArmRot.z, phaseProgress) }, "LArm");
        SetRotation({ lerp(currentRecoverRArmRot.x, defaultArmRot.x, phaseProgress),
                      lerp(currentRecoverRArmRot.y, defaultArmRot.y, phaseProgress),
                      lerp(currentRecoverRArmRot.z, defaultArmRot.z, phaseProgress) }, "RArm");

        SetRotation({ lerp(currentRecoverHeadRot.x, defaultOtherRot.x, phaseProgress),
                      lerp(currentRecoverHeadRot.y, defaultOtherRot.y, phaseProgress),
                      lerp(currentRecoverHeadRot.z, defaultOtherRot.z, phaseProgress) }, "Head");

        if (HitAnim.ElapsedTime >= HitAnim.TotalTime)
        {
            HitAnim.IsRunning = false;
            HitAnim.IsEnd = true;
            HitAnim.ElapsedTime = 0.f;
            HitAnim.Phase = Phase::Action;

            SetRotation({ 0.f, 0.f, 0.f }, "Head");
            SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "RArm");
            SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "LArm");
            SetRotation({ 0.f, 0.f, 0.f }, "RLeg");
            SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
            *IsHit = false;
        }
    }
        break;
    }
}

void Zombie::OnCollisionStay(Object* other)
{
    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();
    auto Stat = GetComponent<InfoComponent<EnemyInfo>>();
    auto transform = GetComponent<TransformComponent>();

    if (objType == ObjectType::Player)
    {
        auto playerStat = other->GetComponent<InfoComponent<PlayerInfo>>();
        auto playertransform = other->GetComponent<TransformComponent>();
        
        auto player = static_cast<Player*>(other);
        if (State == MonsterState::Attack && !IsAttackDamage)
        {
            playerStat->SetHp(playerStat->GetInfo().curHp - Stat->GetInfo().power);
            IsAttackDamage = true;
        }
    }
}

void Zombie::Free()
{
    Monster::Free();
}
