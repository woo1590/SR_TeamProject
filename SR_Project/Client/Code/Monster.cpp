#include "pch.h"
#include "Monster.h"
#include "IsTargetInAttackRange.h"
#include "Chase.h"
#include "Attack.h"
#include "SequenceNode.h"
#include "SelectorNode.h"
#include "ObjectManager.h"
#include "BehaviorTree.h"
#include "BlackBoard.h"
#include "TransformComponent.h"
#include "AIController.h"
#include "IsAlive.h"
#include "Die.h"

Monster::Monster(ObjectManager* owner, ObjectType objType)
	:BaseCharacter(owner, objType)
{
}

Monster::~Monster()
{
}

Monster* Monster::Create(ObjectManager* owner, ObjectType objType)
{
    Monster* Instance = new Monster(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Monster::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    BaseCharacter::Ready_Object(owner, objType);

    Bones["Body"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 6.f * Scale, 0.0f));
    Bones["Body"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    Bones["Head"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 8.f*Scale, 0.f));
    Bones["Head"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    transform->SetPosition(_vec3(-30.f, 30.f, 0.f));        //임의 설정
    
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
    bb->SetValue("Distance", new float(3.f));

    auto AI = AddComponent<AIController>(bt, bb);

    InitAnimation();
	return S_OK;
}

void Monster::Update(_float dt)
{
    BaseCharacter::Update(dt);
    PlayAnimation(dt);
    Hp -= 0.05f;
}

void Monster::Late_Update(_float dt)
{
    BaseCharacter::Late_Update(dt);
}

void Monster::MoveTo(_vec3* dir, _float dt)
{
    if (State != MonsterState::Walk) State = MonsterState::Walk;

    auto Transform = GetComponent<TransformComponent>();
    Transform->Translate((*dir) * dt * Speed);
    //Transform->SetForward(*dir);
}

void Monster::Attack(Object* target)
{
    if (State != MonsterState::Attack) State = MonsterState::Attack;
    //충돌?
}

void Monster::Die()
{
    if (State != MonsterState::Die)
    {
        State = MonsterState::Die;

        DieAnim.ElapsedTime = 0;
        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "LHand");
        SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "RHand");
        SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
        SetRotation({ 0.f, 0.f, 0.f }, "RLeg");
    }
}

void Monster::InitAnimation()
{
    //Walk
    //WalkAnim.Start = ;

    //Attack

    //Die
    DieAnim.Start = 0;                  //start angle
    DieAnim.End = 270.f;                //end angle
    DieAnim.TotalTime = 0.5f;          //play animation total time
}

void Monster::PlayAnimation(_float dt)
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
        if(!AttackAnim.IsRunning) AttackAnim.IsRunning = true;
        PlayAttack(dt);
        break;
    case MonsterState::Die:
        if (!DieAnim.IsRunning) DieAnim.IsRunning = true;
        PlayDie(dt);
        break;
    }
}

void Monster::PlayIdle(_float dt)
{
    //idle animation
}

void Monster::PlayWalk(_float dt)
{
    //walk animation
    WalkAnim.ElapsedTime += dt;

    float Angle = sinf(WalkAnim.ElapsedTime * 10.f);
    SetRotation({Angle, 0.f, 0.f }, "LLeg");
    SetRotation({ -Angle, 0.f, 0.f }, "RLeg");

    SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "LHand");
    SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "RHand");
}

void Monster::PlayAttack(_float dt)
{
    //attack animation
    AttackAnim.ElapsedTime += dt;

    float Angle = sinf(AttackAnim.ElapsedTime * 1.f);

    //Body
    SetRotation({ -Angle/2, 0.f, 0.f });

    //Hand 
    SetRotation({ 180 - Angle*2, 0.f, 0.f }, "LHand");
    SetRotation({ 180 - Angle*2, 0.f, 0.f }, "RHand");

    // Leg
    SetRotation({ Angle / 2, 0.f, 0.f }, "LLeg");
    SetRotation({ Angle / 2, 0.f, 0.f }, "RLeg");
}

void Monster::PlayDie(_float dt)
{
    DieAnim.ElapsedTime += dt;

    _float t = clamp(DieAnim.ElapsedTime / DieAnim.TotalTime, 0.f, 1.f);

    _float Angle = lerp(DieAnim.Start, DieAnim.End, t);

    SetRotation({0.f, D3DXToRadian(Angle), D3DXToRadian(Angle * 0.33f)});

    if (t >= 1.f)
    {
        DieAnim.IsEnd = true;
    }
}

void Monster::Free()
{
    BaseCharacter::Free();
}
