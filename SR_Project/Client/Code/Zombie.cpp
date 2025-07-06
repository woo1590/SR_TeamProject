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
#include "Player.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "AIController.h"
#include "IsAlive.h"
#include "Hit.h"
#include "Die.h"
#include "InfoDetector.h"

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

    //InitTransform
    Bones["Body"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 6.f * Scale, 0.0f));
    Bones["Body"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    Bones["Head"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 8.f * Scale, 0.f));
    Bones["Head"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    auto transform = AddComponent<TransformComponent>();

    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(3.5f, 7.f, 2.5f));
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    transform->SetPosition(_vec3(-5.f, 150.f, -5.f));

    //Create BT
    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    bb->SetValue("Target", owner->GetObjectList(ObjectType::Player).back());
    Distance = new float(3.f);
    bb->SetValue("Distance", Distance);
    PrevHp = new float(100.f);
    bb->SetValue("PrevHp", PrevHp);

    ChaseNode* chase = new ChaseNode();
    AttackNode* attack = new AttackNode();
    IsTargetInAttackRange* attackCheck = new IsTargetInAttackRange(attack);

    SequenceNode* attackSequence = new SequenceNode();
    attackSequence->AddChild(attackCheck);

    SelectorNode* BehaviorNode = new SelectorNode();
    BehaviorNode->AddChild(attackSequence);
    BehaviorNode->AddChild(chase);

    SelectorNode* a = new SelectorNode();
    a->AddChild(new HitNode());
    a->AddChild(BehaviorNode);

    IsAliveNode* IsAlive = new IsAliveNode(a);
    DieNode* die = new DieNode();

    SelectorNode* root = new SelectorNode();
    root->AddChild(IsAlive);
    root->AddChild(die);

    BehaviorTree* bt = BehaviorTree::Create(root);
    
    auto AI = AddComponent<AIController>(bt, bb);

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
    }
    //?תפ?
}

void Zombie::Die()
{
    if (State != MonsterState::Die)
    {
        State = MonsterState::Die;

        DieAnim.ElapsedTime = 0;
        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "LArm");
        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "RArm");
        SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
        SetRotation({ 0.f, 0.f, 0.f }, "RLeg");
    }
}

void Zombie::Hit(_vec3 dir, _float power, _float dt)
{
    if(State != MonsterState::Hit)
    {
        State = MonsterState::Hit;

        HitAnim.IsRunning = true;
        HitAnim.ElapsedTime = 0.f;
        HitAnim.DelayTime = 0.f;
        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "LArm");
        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "RArm");
        SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
        SetRotation({ 0.f, 0.f, 0.f }, "RLeg");
    }
    PlayKnockBack(dir, power, dt);
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
    case MonsterState::Die:
        if (!DieAnim.IsRunning) DieAnim.IsRunning = true;
        PlayDie(dt);
        break;
    case MonsterState::Hit:
        if (!HitAnim.IsRunning) HitAnim.IsRunning = true;
        PlayHit(dt);
        break;
    }
    if (State != MonsterState::Attack)
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
    }
}

void Zombie::PlayDie(_float dt)
{
    DieAnim.ElapsedTime += dt;

    _float t = clamp(DieAnim.ElapsedTime / DieAnim.TotalTime, 0.f, 1.f);

    _float Angle = lerp(DieAnim.Start, DieAnim.End, t);

    SetRotation({ 0.f, D3DXToRadian(Angle), D3DXToRadian(Angle * 0.33f) });

    if (t >= 1.f)
    {
        DieAnim.IsEnd = true;
    }
}

void Zombie::PlayHit(_float dt)
{
    HitAnim.ElapsedTime += dt;

    _float t = clamp(HitAnim.ElapsedTime / HitAnim.TotalTime, 0.f, 1.f);

    _float Angle = lerp(HitAnim.Start, HitAnim.End, t);

    SetRotation({ -D3DXToRadian(Angle/3), 0.f, 0.f }, "Head");
    SetRotation({ 0.f, 0.f, -D3DXToRadian(Angle) }, "LArm");
    SetRotation({ 0.f, 0.f, D3DXToRadian(Angle) }, "RArm");
    SetRotation({ 0.f, D3DXToRadian(Angle), D3DXToRadian(Angle/ 2) }, "RLeg");

    if (HitAnim.ElapsedTime > HitAnim.TotalTime)
    {
        HitAnim.IsRunning = false;
        HitAnim.IsEnd = true;
        HitAnim.ElapsedTime = 0.f;

        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "LArm");
        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "RArm");
        SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
        SetRotation({ 0.f, 0.f, 0.f }, "RLeg");
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
        if (State == MonsterState::Attack)
        {
            Stat->SetHp(Stat->GetInfo().curHp - Stat->GetInfo().power);
            //playerStat->SetHp(playerStat->GetInfo().curHp - Stat->GetInfo().power);
            //static_cast<Player*>(other)->PlayKnockBack(playertransform->GetPosition() - transform->GetPosition(), Stat->GetInfo().power, 0.1f);

            //collision->ResolveAABBColiision(other);
        }
    }
}

void Zombie::Free()
{
    Monster::Free();
}
