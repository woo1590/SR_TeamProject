#include "pch.h"
#include "PurpleBabySlime.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "InfoComponent.h"
#include "Chase.h"
#include "Attack.h"
#include "SequenceNode.h"
#include "SelectorNode.h"
#include "AIController.h"
#include "IsAlive.h"
#include "Die.h"
#include "IsTargetInAttackRange.h"
#include "ObjectManager.h"
#include "PhysicsComponent.h"
#include "MeshRendererComponent.h"
#include "Player.h"
#include "DeadEffect.h"
#include "SoundManager.h"
#include "EngineCore.h"

PurpleBabySlime::PurpleBabySlime(ObjectManager* owner, ObjectType objType)
    :Monster(owner, objType)
{
}

PurpleBabySlime::~PurpleBabySlime()
{
}

PurpleBabySlime* PurpleBabySlime::Create(ObjectManager* owner, ObjectType objType)
{
    PurpleBabySlime* Instance = new PurpleBabySlime(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT PurpleBabySlime::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Monster::Ready_Object(owner, objType);

    InitTransform(objType);

    //Init Collision
    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(4.f, 4.f, 4.f));

    //Create BT
    InitTree();

    //Animation
    InitAnimation();
    return S_OK;
}

void PurpleBabySlime::Update(_float dt)
{
    Monster::Update(dt);
    PlayAnimation(dt);
}

void PurpleBabySlime::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void PurpleBabySlime::MoveTo(_vec3* dir, _float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    auto Stat = GetComponent<InfoComponent<EnemyInfo>>();

    if (State != MonsterState::Walk) State = MonsterState::Walk;
    D3DXVec3Normalize(dir, dir);
    Transform->Translate(*dir * dt * Stat->GetInfo().speed * 0.6);
    Transform->SetForward(_vec3(dir->x, 0.f, dir->z));
}

void PurpleBabySlime::RotateTo(_vec3* dir, float dt)
{
}

void PurpleBabySlime::Attack(Object* target)
{
    if (State != MonsterState::Attack)
    {
        State = MonsterState::Attack;
        AttackAnim.DelayTime = 0.f;
        AttackAnim.ElapsedTime = 0;
        SetRotation({ 0.f, 0.f, 0.f }, "Body");
        *IsAttack = true;
        IsAttackDamage = false;
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("AttackSlime");
    }
}

void PurpleBabySlime::Die()
{
    if (State != MonsterState::Die)
    {
        State = MonsterState::Die;
        DieAnim.IsRunning = true;
        DieAnim.IsEnd = false;
    }
}

void PurpleBabySlime::Hit(_vec3 dir, _float power)
{
    if (State != MonsterState::Hit)
    {
        State = MonsterState::Hit;

        *IsHit = true;

        HitDir = dir;
        HitPower = power * 0.5;

        Monster::Hit(dir, power * 0.5);
    }
}

void PurpleBabySlime::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    transform->SetPosition(_vec3(5.f, 100.f, 5.f));
    SetMaterial("PurpleSlimeOut_Mtrl", "Body", RENDER_ID::Render_Alpha);
    SetMaterial("PurpleSlimeIn_Mtrl", "Head", RENDER_ID::Render_Alpha);

    Bones["LArm"]->SetDead();
    Bones["LArm"] = nullptr;
    Bones["RArm"]->SetDead();
    Bones["RArm"] = nullptr;
    Bones["LLeg"]->SetDead();
    Bones["LLeg"] = nullptr;
    Bones["RLeg"]->SetDead();
    Bones["RLeg"] = nullptr;

    Scale = 0.1f;
    SetScale(_vec3(7.f * Scale, 7.f * Scale, 7.f * Scale), "Head");
    SetPosition(_vec3(0.f * Scale, 0.f * Scale, 0.f * Scale), "Head");
    SetScale(_vec3(10.f * Scale, 10.f * Scale, 10.f * Scale), "Body");
    //body
}

void PurpleBabySlime::InitTree()
{
    //blackboard
    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    bb->SetValue("Target", owner->GetObjectList(ObjectType::Player).back());
    Distance = new float(3.5f);
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

void PurpleBabySlime::InitAnimation()
{
    WalkAnim.Phase = Action;
    WalkAnim.ElapsedTime = 0.f;
    WalkAnim.TotalTime = 0.3f;
    WalkAnim.IsRunning = true;

    AttackAnim.Phase = Action;
    AttackAnim.ElapsedTime = 0.f;
    AttackAnim.TotalTime = 0.2f;

    HitAnim.ElapsedTime = 0.f;
    HitAnim.TotalTime = 0.2f;

    DieAnim.ElapsedTime = 0.f;
    DieAnim.TotalTime = 0.3f;
}

void PurpleBabySlime::PlayAnimation(_float dt)
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
    if (State != MonsterState::Attack)
    {
        auto Transform = GetComponent<TransformComponent>();
        SetRotation(_vec3(Transform->GetRotate().x, 0.f, Transform->GetRotate().z), "Body");
    }
}

void PurpleBabySlime::PlayIdle(_float dt)
{
}

void PurpleBabySlime::PlayWalk(_float dt)
{
    WalkAnim.ElapsedTime += dt;
    _float t = clamp(WalkAnim.ElapsedTime / WalkAnim.TotalTime, 0.f, 1.f);

    auto physics = GetComponent<PhysicsComponent>();
    switch (WalkAnim.Phase)
    {
    case Phase::Action:
    {
        _float curSize = lerp(1.5f, 1.f, t);
        SetScale(_vec3(curSize, curSize, curSize), "Body");
        if (t >= 1.f)
        {
            WalkAnim.Phase = Phase::Recover;
            WalkAnim.ElapsedTime = 0.f;
            WalkAnim.TotalTime = 0.3f;
        }
        break;
    }
    case Phase::Recover:
    {
        _float curSize = lerp(1.f, 1.5f, t);
        SetScale(_vec3(curSize, curSize, curSize), "Body");
        if (physics->IsGrounded())
        {
            physics->SetGround(false);
            physics->SetVelocity(_vec3(0.0f, 12.f, 0.f));
        }

        if (t >= 1.f)
        {
            WalkAnim.Phase = Phase::Action;
            WalkAnim.ElapsedTime = 0.f;
            WalkAnim.TotalTime = 0.3f;
        }
        break;
    }
    }
}

void PurpleBabySlime::PlayAttack(_float dt)
{
    AttackAnim.DelayTime -= dt;
    if (AttackAnim.DelayTime > 0) return;

    AttackAnim.ElapsedTime += dt;
    _float t = clamp(AttackAnim.ElapsedTime / AttackAnim.TotalTime, 0.f, 1.f);

    switch (AttackAnim.Phase)
    {
    case Phase::Action:
    {
        _float Angle = lerp(0.f, 40.f, t);

        SetRotation(_vec3(D3DXToRadian(Angle), 0.f, 0.f), "Body");

        if (t >= 1.f)
        {
            AttackAnim.Phase = Phase::Recover;
            AttackAnim.ElapsedTime = 0.f;
            AttackAnim.TotalTime = 0.2f;
        }
        break;
    }
    case Phase::Recover:
    {
        _float Angle = lerp(40.f, 0.f, t);
        SetRotation(_vec3(D3DXToRadian(Angle), 0.f, 0.f), "Body");
        if (t >= 1.f)
        {
            AttackAnim.Phase = Phase::Action;
            AttackAnim.ElapsedTime = 0.f;
            AttackAnim.TotalTime = 0.2f;
            AttackAnim.DelayTime = 1.f;
            *IsAttack = false;
        }
        break;
    }
    }
}

void PurpleBabySlime::PlayDie(_float dt)
{
    DieAnim.ElapsedTime += dt;
    if (DieAnim.ElapsedTime > DieAnim.TotalTime && DieAnim.IsEnd == false)
    {
        //////////////////////////////////Dead Effect
        auto effect = DeadEffect::Create(owner, ObjectType::ParticleEffect);
        effect->GetComponent<TransformComponent>()->SetPosition(GetComponent<TransformComponent>()->GetPosition());
        effect->SetDeadTime(1.5f);
        owner->AddObject(ObjectType::ParticleEffect, effect);
        //////////////////////////////////
        DropEmeralds();
        SetDead();
        DeleteBar();

        DieAnim.IsEnd = true;
    }
}

void PurpleBabySlime::PlayHit(_float dt)
{
    HitAnim.ElapsedTime += dt;

    PlayKnockBack(HitDir, HitPower, dt);

    if (HitAnim.ElapsedTime >= HitAnim.TotalTime)
    {
        *IsHit = false;
    }
}

void PurpleBabySlime::OnCollisionStay(Object* other)
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
            playerStat->SetHp(playerStat->GetInfo().curHp - Stat->GetInfo().power * 0.5);
            IsAttackDamage = true;
        }
    }
}

void PurpleBabySlime::Free()
{
    Monster::Free();
}
