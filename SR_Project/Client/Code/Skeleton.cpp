#include "pch.h"
#include "Skeleton.h"
#include "TransformComponent.h"
#include "IsTargetInAttackRange.h"
#include "Chase.h"
#include "Attack.h"
#include "SequenceNode.h"
#include "SelectorNode.h"
#include "ObjectManager.h"
#include "BehaviorTree.h"
#include "BlackBoard.h"
#include "CollisionComponent.h"
#include "InfoComponent.h"
#include "AIController.h"
#include "IsAlive.h"
#include "Rotate.h"
#include "Die.h"

Skeleton::Skeleton(ObjectManager* owner, ObjectType objType)
	:Monster(owner, objType)
{
}

Skeleton::~Skeleton()
{
}

Skeleton* Skeleton::Create(ObjectManager* owner, ObjectType objType)
{
    Skeleton* Instance = new Skeleton(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Skeleton::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Monster::Ready_Object(owner, objType);

    //InitTransform
    InitTransform(objType);

    //Init Collision
    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(2.f, 7.f, 2.f));

    //Create BT
    InitTree();

    //Animation
    InitAnimation();
	return S_OK;
}

void Skeleton::Update(_float dt)
{
    Monster::Update(dt);
    PlayAnimation(dt);
}

void Skeleton::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void Skeleton::MoveTo(_vec3* dir, _float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    auto Stat = GetComponent<InfoComponent<EnemyInfo>>();

    if (State != MonsterState::Walk) State = MonsterState::Walk;
    D3DXVec3Normalize(dir, dir);
    Transform->Translate(*dir * dt * Stat->GetInfo().speed);
    Transform->SetForward(_vec3(dir->x, 0.f, dir->z));
}

void Skeleton::RotateTo(_vec3* dir, float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    Transform->SetForward(_vec3(dir->x, 0.f, dir->z));
}

void Skeleton::Attack(Object* target)
{
    if (State != MonsterState::Attack)
    {
        State = MonsterState::Attack;
        AttackAnim.DelayTime = 0.f;
        AttackAnim.ElapsedTime = 0;
        SetPosition(_vec3(9.f * Scale, 5.f * Scale, 5.f * Scale), "RArm");
        SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
        SetRotation({ 0.f, 0.f, 0.f }, "RLeg");
    }
}

void Skeleton::Die()
{
    if (State != MonsterState::Die)
    {
        State = MonsterState::Die;

        DieAnim.ElapsedTime = 0;
    }
}


void Skeleton::Hit(_vec3 dir, _float power)
{
    if (State != MonsterState::Hit)
    {
        State = MonsterState::Hit;

        HitAnim.IsRunning = true;
        HitAnim.IsEnd = false;
        HitAnim.ElapsedTime = 0.f;
        HitAnim.DelayTime = 0.f;
        SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
        SetRotation({ 0.f, 0.f, 0.f }, "RLeg");

        HitDir = dir;
        HitPower = power;
    }
}

void Skeleton::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    transform->SetPosition(_vec3(30.f, 100.f, 40.f));
    SetMaterial(L"SkeletonBody_Mtrl", "Body", RENDER_ID::Render_Alpha);
    SetMaterial(L"SkeletonFace_Mtrl", "Head", RENDER_ID::Render_Alpha);
    SetMaterial(L"SkeletonBone_Mtrl", "LArm");
    SetMaterial(L"SkeletonBone_Mtrl", "RArm");
    SetMaterial(L"SkeletonBone_Mtrl", "LLeg");
    SetMaterial(L"SkeletonBone_Mtrl", "RLeg");

    //head
    SetScale(_vec3(7.f * Scale, 7.f * Scale, 7.f * Scale), "Head");
    SetPosition(_vec3(0.f, 17.5f * Scale, 0.f), "Head");

    //body
    SetScale(_vec3(8.f * Scale, 12.f * Scale, 2.f * Scale), "Body");

    //arm
    SetScale(_vec3(2.f * Scale, 5 * Scale, 2.f * Scale), "LArm");
    SetPosition(_vec3(-9.f * Scale, 5.f * Scale, 0.f), "LArm");
    SetScale(_vec3(2.f * Scale, 5 * Scale, 2.f * Scale), "RArm");
    SetPosition(_vec3(9.f * Scale, 5.f * Scale, 0.f), "RArm");

    //leg
    SetScale(_vec3(2.f * Scale, 12.f * Scale, 2.f * Scale), "LLeg");
    SetScale(_vec3(2.f * Scale, 12.f * Scale, 2.f * Scale), "RLeg");

    //hand
    Add_Bone("LHand", objType, Bones["LArm"], L"SkeletonBone_Mtrl");
    SetScale(_vec3(2.f * Scale, 5 * Scale, 2.f * Scale), "LHand");
    SetPosition(_vec3(0.f, -10 * Scale, 0.f), "LHand");

    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 5.f * Scale, 0.0f));
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    Add_Bone("RHand", objType, Bones["RArm"], L"SkeletonBone_Mtrl");
    SetScale(_vec3(2.f * Scale, 5 * Scale, 2.f * Scale), "RHand");
    SetPosition(_vec3(0.f, -10 * Scale, 0.f), "RHand");

    Bones["RHand"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 5.f * Scale, 0.0f));
    Bones["RHand"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    SetWeapon(Bones["LArm"], objType, L"Bow_Mtrl");
    SetPosition(_vec3(0.f, -13 * Scale, 1.f * Scale), "Weapon");
    SetRotation(_vec3(-90.f, 0.f, 0.f), "Weapon");
}

void Skeleton::InitTree()
{
    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    bb->SetValue("Target", owner->GetObjectList(ObjectType::Player).back());
    Distance = new float(15.f);
    bb->SetValue("Distance", Distance);
    IsHit = new _bool(false);
    bb->SetValue("IsDamaged", IsHit);

    SequenceNode* rotateThenAttack = new SequenceNode();
    rotateThenAttack->AddChild(new RotateNode());
    rotateThenAttack->AddChild(new AttackNode());

    IsTargetInAttackRange* attackCheck = new IsTargetInAttackRange(rotateThenAttack);

    SelectorNode* attackBehavior = new SelectorNode();
    attackBehavior->AddChild(attackCheck);
    attackBehavior->AddChild(new ChaseNode());

    IsAliveNode* isAlive = new IsAliveNode(attackBehavior);

    SelectorNode* root = new SelectorNode();
    root->AddChild(isAlive);
    root->AddChild(new DieNode());

    BehaviorTree* bt = BehaviorTree::Create(root);

    auto AI = AddComponent<AIController>(bt, bb);
}

void Skeleton::InitAnimation()
{
    //Walk
    WalkAnim.ElapsedTime = 0.f;

    //Attack
    AttackAnim.TotalTime = 1.5f;
    AttackAnim.ElapsedTime = 0.f;
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

void Skeleton::PlayAnimation(_float dt)
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

void Skeleton::PlayIdle(_float dt)
{
}

void Skeleton::PlayWalk(_float dt)
{
    WalkAnim.ElapsedTime += dt;

    float Angle = sinf(WalkAnim.ElapsedTime * 10.f);
    SetRotation({ Angle, 0.f, 0.f }, "LLeg");
    SetRotation({ -Angle, 0.f, 0.f }, "RLeg");

    SetRotation({ -Angle, 0.f, 0.f }, "LArm");
    SetRotation({ Angle, 0.f, 0.f }, "RArm");
}

void Skeleton::PlayAttack(_float dt)
{
    AttackAnim.DelayTime -= dt;
    if (AttackAnim.DelayTime > 0.f) return;

    AttackAnim.ElapsedTime += dt;

    float t = clamp(AttackAnim.ElapsedTime / AttackAnim.TotalTime, 0.f, 1.f);
    float angle = sinf(t * D3DX_PI); 

    SetRotation({ D3DXToRadian(-90.f), D3DXToRadian(-10.f), D3DXToRadian(0.f) }, "LArm");
    SetRotation({ D3DXToRadian(-10.f), 0.f, 0.f }, "LHand");

    float pullAngle = lerp(0.f, D3DXToRadian(-120.f), t); 
    SetRotation({ D3DXToRadian(-20.f), D3DXToRadian(0.f), 0.f }, "RArm"); 
    SetPosition(_vec3(9.f * Scale, 5.f * Scale, 5.f * Scale - 0.5f * t),"RArm");
    SetRotation({ pullAngle, 0.f, 0.f }, "RHand"); 

    SetRotation({ D3DXToRadian(angle * 5.f), 0.f, 0.f }, "Head");

    if (AttackAnim.ElapsedTime > AttackAnim.TotalTime)
    {
        AttackAnim.IsRunning = false;
        AttackAnim.IsEnd = true;
        AttackAnim.ElapsedTime = 0.f;
        AttackAnim.DelayTime = 1.5f;

        SetRotation({ 0.f, 0.f, 0.f }, "Head");
        SetRotation({ 0.f, 0.f, 0.f }, "LHand");
        SetRotation({ 0.f, 0.f, 0.f }, "RHand");
        SetRotation({ 0.f, 0.f, 0.f }, "LArm");
        SetRotation({ 0.f, 0.f, 0.f }, "RArm");
        SetPosition(_vec3(9.f * Scale, 5.f * Scale, 0.f), "RArm");
    }
}

void Skeleton::PlayHit(float dt)
{
    HitAnim.ElapsedTime += dt;

    _float t = clamp(HitAnim.ElapsedTime / HitAnim.TotalTime, 0.f, 1.f);

    _float Angle = lerp(HitAnim.Start, HitAnim.End, t);

    SetRotation({ -D3DXToRadian(Angle / 3), 0.f, 0.f }, "Head");
    SetRotation({ 0.f, 0.f, -D3DXToRadian(Angle) }, "LArm");
    SetRotation({ 0.f, 0.f, -D3DXToRadian(Angle) }, "RArm");
    SetRotation({ 0.f, D3DXToRadian(Angle), D3DXToRadian(Angle / 2) }, "RLeg");

    PlayKnockBack(HitDir, HitPower, dt);

    if (HitAnim.ElapsedTime > HitAnim.TotalTime)
    {
        HitAnim.IsRunning = false;
        HitAnim.IsEnd = true;
        HitAnim.ElapsedTime = 0.f;

        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "LArm");
        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "RArm");
        SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
        SetRotation({ 0.f, 0.f, 0.f }, "RLeg");
        *IsHit = false;
    }
}

void Skeleton::PlayDie(_float dt)
{
    if (!DieAnim.IsEnd)
    {
        auto transform = GetComponent<TransformComponent>();
        _vec3 pos = transform->GetPosition();
        for (auto& bone : Bones)
        {
            auto transform = bone.second->GetComponent< TransformComponent>();
            _vec3 bonepos = transform->GetPosition();

            DetachParent(bone.first);
            transform->SetPosition(pos + bonepos);
        }
        DieAnim.IsEnd = true;
        DieAnim.IsRunning = false;
    }
}

void Skeleton::OnCollisionStay(Object* other)
{
    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();
    auto Stat = GetComponent<InfoComponent<EnemyInfo>>();
    auto transform = GetComponent<TransformComponent>();

    if (objType == ObjectType::Player)
    {
        auto playerStat = other->GetComponent<InfoComponent<PlayerInfo>>();
        auto playertransform = other->GetComponent<TransformComponent>();
        /* if (State == MonsterState::Attack)
         {*/
         //playerStat->SetHp(playerStat->GetInfo().curHp - Stat->GetInfo().power);
        Stat->SetHp(Stat->GetInfo().curHp - Stat->GetInfo().power);
        *IsHit = true;
        Hit(transform->GetPosition() - playertransform->GetPosition(), playerStat->GetInfo().power);
        //static_cast<Player*>(other)->PlayKnockBack(playertransform->GetPosition() - transform->GetPosition(), Stat->GetInfo().power, 0.1f);

        //collision->ResolveAABBColiision(other);
    //}
    }
}

void Skeleton::Free()
{
    Monster::Free();
}
