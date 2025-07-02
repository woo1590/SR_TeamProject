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
#include "CollisionComponent.h"
#include "AIController.h"

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

    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(_vec3(-100.f, 0.f, 0.f));        //임의 설정

    auto collision = AddComponent<CollisionComponent>();
    collision->SetSize(_vec3(2.f, 7.f, 2.f));
    
    ChaseNode* chase = new ChaseNode();
    AttackNode* attack = new AttackNode();
    IsTargetInAttackRange* attackCheck = new IsTargetInAttackRange(attack);

    SequenceNode* attackSequence = new SequenceNode();
    attackSequence->AddChild(attackCheck); 

    SelectorNode* root = new SelectorNode();
    root->AddChild(attackSequence);
    root->AddChild(chase);  

    BehaviorTree* bt = BehaviorTree::Create(root);

    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    bb->SetValue("Target", owner->GetObjectList(ObjectType::Player).back());
    bb->SetValue("Distance", new float(3.f));

    auto AI = AddComponent<AIController>(bt, bb);

    for (auto& pair : Bones) 
    {
        auto pBoneTransform = pair.second->GetComponent<TransformComponent>();
        pBoneTransform->SetParent(transform);
    }

	return S_OK;
}

void Monster::Update(_float dt)
{
    BaseCharacter::Update(dt);
    PlayAnimation(dt);
}

void Monster::Late_Update(_float dt)
{
    BaseCharacter::Late_Update(dt);
}

void Monster::MoveTo(_vec3* dir)
{
    if (State != MonsterState::Walk) State = MonsterState::Walk;

    auto Transform = GetComponent<TransformComponent>();
    Transform->Translate((*dir) * Speed);
    Transform->SetForward(*dir);
}

void Monster::Attack(Object* target)
{
    if (State != MonsterState::Attack) 
        State = MonsterState::Attack;
    //충돌?
}

void Monster::PlayAnimation(_float dt)
{
    if (IsAttackFinished) AttackDelay -= dt;

    switch (State)
    {
    case MonsterState::Idle:
        PlayIdle(dt);
        break;
    case MonsterState::Walk:
        PlayWalk(dt);
        break;
    case MonsterState::Attack:
        if (AttackDelay > 0.f) 
            break;
        PlayAttack(dt);
        IsAttacking = true;
        IsAttackFinished = false;
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
    WalkTime += dt;

    float Angle = sinf(WalkTime * 10.f);
    SetRotation({Angle, 0.f, 0.f }, "LLeg");
    SetRotation({ -Angle, 0.f, 0.f }, "RLeg");

    SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "LHand");
    SetRotation({ D3DXToRadian(-90.f), 0.f, 0.f }, "RHand");
}

void Monster::PlayAttack(_float dt)
{
    //attack animation
    AttackTime += dt;

    float Angle = sinf(AttackTime * 10.f);

    //Body
    SetRotation({ -Angle/2, 0.f, 0.f });
    SetRotation({ -Angle/2, 0.f, 0.f }, "Head");

    //Hand
    SetRotation({ 180 - Angle*2, 0.f, 0.f }, "LHand");
    SetRotation({ 180 - Angle*2, 0.f, 0.f }, "RHand");

    //// 수정예정 - 다리고정
    SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
    SetRotation({ 0.f, 0.f, 0.f }, "RLeg");

    //if (AttackTime > 2.f)
    //{
    //    IsAttacking = false;
    //    IsAttackFinished = true;
    //    AttackDelay = 2.f;
    //}
}

void Monster::PlayDie(_float dt)
{
    //die animation
}

void Monster::Free()
{
    BaseCharacter::Free();
}
