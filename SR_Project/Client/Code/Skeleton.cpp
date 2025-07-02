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
#include "AIController.h"
#include "IsAlive.h"
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
    BaseCharacter::Ready_Object(owner, objType);

    //InitTransform
    SetMaterial(L"SkeletonBody_Mtrl", "Body", RENDER_ID::Render_Alpha);
    SetMaterial(L"SkeletonFace_Mtrl", "Head", RENDER_ID::Render_Alpha);
    SetMaterial(L"SkeletonBone_Mtrl", "LArm");
    SetMaterial(L"SkeletonBone_Mtrl", "RArm");
    SetMaterial(L"SkeletonBone_Mtrl", "LLeg");
    SetMaterial(L"SkeletonBone_Mtrl", "RLeg");

    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);
    transform->SetPosition(_vec3(-20.f, 0.f, 0.f));

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
   
    //Create BT
    ChaseNode* chase = new ChaseNode();
    AttackNode* attack = new AttackNode();
    IsTargetInAttackRange* attackCheck = new IsTargetInAttackRange(attack);

    SequenceNode* attackSequence = new SequenceNode();
    attackSequence->AddChild(attackCheck);

    SelectorNode* BehaviorNode = new SelectorNode();
    BehaviorNode->AddChild(attackSequence);
    BehaviorNode->AddChild(chase);

    IsAliveNode* IsAlive = new IsAliveNode(BehaviorNode);
    DieNode* die = new DieNode();

    SelectorNode* root = new SelectorNode();
    root->AddChild(IsAlive);
    root->AddChild(die);

    BehaviorTree* bt = BehaviorTree::Create(root);

    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    bb->SetValue("Target", owner->GetObjectList(ObjectType::Player).back());
    bb->SetValue("Distance", new float(15.f));

    auto AI = AddComponent<AIController>(bt, bb);
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
    if (State != MonsterState::Walk) State = MonsterState::Walk;
    D3DXVec3Normalize(dir, dir);
    Transform->Translate(*dir * dt * Speed);
    Transform->SetForward(_vec3(dir->x, 0.f, dir->z));
}

_vec3 Skeleton::RotateTo(_vec3* dir, float dt)
{
    return _vec3();
}

void Skeleton::Attack(Object* target)
{
    if (State != MonsterState::Attack)
    {
        State = MonsterState::Attack;
        AttackAnim.DelayTime = 0.f;
        AttackAnim.ElapsedTime = 0;
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

void Skeleton::InitAnimation()
{
    //Walk
    //WalkAnim.Start = ;

    //Attack
    AttackAnim.TotalTime = 1.5f;
    AttackAnim.DelayTime = 0.f;

    //Die
    DieAnim.Start = 0;                  //start angle
    DieAnim.End = 270.f;                //end angle
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
    SetPosition(_vec3(9.f * Scale, 5.f * Scale, - 0.5f * t),"RArm");
    SetRotation({ pullAngle, 0.f, 0.f }, "RHand"); 

    SetRotation({ D3DXToRadian(angle * 5.f), 0.f, 0.f }, "Head");

    if (AttackAnim.ElapsedTime > AttackAnim.TotalTime)
    {
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

void Skeleton::PlayDie(_float dt)
{
    //bone detach
}

void Skeleton::Free()
{
    Monster::Free();
}
