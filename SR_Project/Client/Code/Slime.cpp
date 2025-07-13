#include "pch.h"
#include "Slime.h"
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

Slime::Slime(ObjectManager* owner, ObjectType objType)
	:Monster(owner, objType)
{
}

Slime::~Slime()
{
}

Slime* Slime::Create(ObjectManager* owner, ObjectType objType)
{
    Slime* Instance = new Slime(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Slime::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Monster::Ready_Object(owner, objType);

    InitTransform(objType);

    //Init Collision
    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(10.f, 10.f, 10.f));

    //Create BT
    InitTree();

    //Animation
    InitAnimation();
    return S_OK;
}

void Slime::Update(_float dt)
{
    Monster::Update(dt);
    PlayAnimation(dt);
}

void Slime::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void Slime::MoveTo(_vec3* dir, _float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    auto Stat = GetComponent<InfoComponent<EnemyInfo>>();

    if (State != MonsterState::Walk) State = MonsterState::Walk;
    D3DXVec3Normalize(dir, dir);
    Transform->Translate(*dir * dt * Stat->GetInfo().speed *0.6);
    Transform->SetForward(_vec3(dir->x, 0.f, dir->z));
}

void Slime::RotateTo(_vec3* dir, float dt)
{
}

void Slime::Attack(Object* target)
{
}

void Slime::Die()
{
}

void Slime::Hit(_vec3 dir, _float power)
{
}

void Slime::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    transform->SetPosition(_vec3(5.f, 100.f, 5.f));
    SetMaterial("SlimeOut_Mtrl", "Body", RENDER_ID::Render_NonAlpha);
    SetMaterial("SlimeIn_Mtrl", "Head", RENDER_ID::Render_Alpha);

    Bones["LArm"]->SetDead();
    Bones["RArm"]->SetDead();
    Bones["LLeg"]->SetDead();
    Bones["RLeg"]->SetDead();

    //head
    Scale = 0.35f;
    SetScale(_vec3(7.f * Scale, 7.f * Scale, 7.f * Scale), "Head");
    SetPosition(_vec3(0.f * Scale, 0.f * Scale, 0.f * Scale), "Head");
    //body
    SetScale(_vec3(10.f * Scale, 10.f * Scale, 10.f * Scale), "Body");
}

void Slime::InitTree()
{
    //blackboard
    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    bb->SetValue("Target", owner->GetObjectList(ObjectType::Player).back());
    Distance = new float(5.f);
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

void Slime::InitAnimation()
{
    WalkAnim.Phase = Action;
    WalkAnim.ElapsedTime = 0.f;
    WalkAnim.TotalTime = 0.3f;
    WalkAnim.IsRunning = true;
}

void Slime::PlayAnimation(_float dt)
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

void Slime::PlayIdle(_float dt)
{
}

void Slime::PlayWalk(_float dt)
{
    WalkAnim.ElapsedTime += dt;
    _float t = clamp(WalkAnim.ElapsedTime / WalkAnim.TotalTime, 0.f, 1.f);

    auto physics = GetComponent<PhysicsComponent>();
    switch (WalkAnim.Phase)
    {
    case Phase::Action:
    {
        _float curSize = lerp(4.5f, 3.5f, t);
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
        _float curSize = lerp(3.5f, 4.5f, t);
        SetScale(_vec3(curSize, curSize, curSize), "Body");
        if (physics->IsGrounded())
        {
            physics->SetGround(false);
            physics->SetVelocity(_vec3(0.0f, 20.f, 0.f));
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

void Slime::PlayAttack(_float dt)
{
    //
}

void Slime::PlayDie(_float dt)
{
}

void Slime::OnCollisionStay(Object* other)
{
}

void Slime::Free()
{
    Monster::Free();
}
