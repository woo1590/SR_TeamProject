#include "pch.h"
#include "Shulker.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "InfoComponent.h"
#include "AIController.h"
#include "IsAlive.h"
#include "Die.h"
#include "BehaviorTree.h"
#include "Attack.h"
#include "IsTargetInAttackRange.h"
#include "BlackBoard.h"
#include "ObjectManager.h"
#include "SequenceNode.h"
#include "SelectorNode.h"
#include "ShulkerBullet.h"
#include "PhysicsComponent.h"
#include "EngineCore.h"
#include "SoundManager.h"

Shulker::Shulker(ObjectManager* owner, ObjectType objType)
	:Monster(owner, objType)
{
}

Shulker::~Shulker()
{
}

Shulker* Shulker::Create(ObjectManager* owner, ObjectType objType)
{
    Shulker* Instance = new Shulker(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Shulker::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Monster::Ready_Object(owner, objType);

    InitTransform(objType);

    //Init Collision
    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(3.f, 3.5f, 3.f));

    //Create BT
    InitTree();

    //Animation
    InitAnimation();

    InitBullet();
    GetComponent<InfoComponent<EnemyInfo>>()->SetInfo({ 1,100,100, 0, 0, 15, 0, 6 });
    
    return S_OK;
}

void Shulker::Update(_float dt)
{
    Monster::Update(dt);
    PlayAnimation(dt);
}

void Shulker::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void Shulker::Attack(Object* target)
{
    if (State != MonsterState::Attack)
    {
        State = MonsterState::Attack;
        AttackAnim.DelayTime = 0.f;
        AttackAnim.ElapsedTime = 0;

        AttackAnim.IsRunning = true;
        AttackAnim.IsEnd = false;
        *IsAttack = true;
    }
}

void Shulker::Die()
{
    if (State != MonsterState::Die)
    {
        State = MonsterState::Die;

        DieAnim.IsRunning = true;
        DieAnim.IsEnd = false;

        auto collision = GetComponent<CollisionComponent>();
        collision->SetSize(_vec3(0.1f, 0.1f, 0.1f));

        auto physics = GetComponent<PhysicsComponent>();
        physics->SetGround(false);

        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("DeathShulker");
    }
}

void Shulker::InitBullet()
{
    for (int i = 0; i < 10; ++i)
    {
        auto bullet = ShulkerBullet::Create(owner, ObjectType::Projectile);
        bullet->SetActive(false);
        bullets.push_back(bullet);
        owner->AddObject(ObjectType::Projectile, bullet);
    }
}

void Shulker::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    SetMaterial("ShulkerFace_Mtrl", "Body", RENDER_ID::Render_NonAlpha);
    SetMaterial("ShulkerTop_Mtrl", "Head", RENDER_ID::Render_Alpha);
    Bones["Head"]->GetComponent<TransformComponent>()->SetParent(transform);

    Add_Bone("BottomHead", ObjectType::Bone, Bones["Body"], "ShulkerBottom_Mtrl");
    SetMaterial("ShulkerBottom_Mtrl", "BottomHead", RENDER_ID::Render_Alpha);
    Bones["BottomHead"]->GetComponent<TransformComponent>()->SetParent(transform);

    SetScale(_vec3(10.f * Scale, 10.f * Scale, 10.f * Scale), "Head");
    SetPosition(_vec3(0.f * Scale, -2.f * Scale, 0.f * Scale), "Head");
    //body
    SetScale(_vec3(5.f * Scale, 5.f * Scale, 5.f * Scale), "Body");
    SetScale(_vec3(10.f * Scale, 10.f * Scale, 10.f * Scale), "BottomHead");
    SetPosition(_vec3(0.f * Scale, -2.f * Scale, 0.f * Scale), "BottomHead");

    Bones["LArm"]->SetDead();
    Bones["LArm"] = nullptr;
    Bones["RArm"]->SetDead();
    Bones["RArm"] = nullptr;
    Bones["LLeg"]->SetDead();
    Bones["LLeg"] = nullptr;
    Bones["RLeg"]->SetDead();
    Bones["RLeg"] = nullptr;
}

void Shulker::InitTree()
{
    //blackboard
    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    bb->SetValue("Target", owner->GetObjectList(ObjectType::Player).back());
    Distance = new float(60.f);
    bb->SetValue("Distance", Distance);
    IsAttack = new _bool(false);
    bb->SetValue("IsAttack", IsAttack);

    //BT
    SelectorNode* BehaviorNode = new SelectorNode();
    BehaviorNode->AddChild(new AttackNode());

    IsAliveNode* IsAlive = new IsAliveNode(BehaviorNode);

    SelectorNode* root = new SelectorNode();
    root->AddChild(IsAlive);
    root->AddChild(new DieNode());

    BehaviorTree* bt = BehaviorTree::Create(root);

    //AI
    auto AI = AddComponent<AIController>(bt, bb);
}

void Shulker::InitAnimation()
{
    AttackAnim.ElapsedTime = 0.f;
    AttackAnim.TotalTime = 0.5f;
    AttackAnim.Phase = Ready;
    AttackAnim.DelayTime = 0.f;

    DieAnim.Start = 0;                  //start angle
    DieAnim.End = 90.f;                //end angle
    DieAnim.ElapsedTime = 0.f;
    DieAnim.TotalTime = 0.7f;
}

void Shulker::PlayAnimation(_float dt)
{
    switch (State)
    {
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

void Shulker::PlayAttack(_float dt)
{
    switch (AttackAnim.Phase)
    {
    case Ready:
    {
        AttackAnim.DelayTime -= dt;
        if (AttackAnim.DelayTime > 0.f) return;

        AttackAnim.ElapsedTime += dt;

        _float t = clamp(AttackAnim.ElapsedTime / AttackAnim.TotalTime, 0.f, 1.f);
        _float pos = lerp(-2.f, 15.f, t);
        _float Angle = lerp(0.f, 90.f, t);
        Bones["Head"]->GetComponent<TransformComponent>()->SetPosition(_vec3(0.f * Scale, pos * Scale, 0.f * Scale));
        Bones["Head"]->GetComponent<TransformComponent>()->SetRotate(_vec3(0.f, D3DXToRadian(Angle), 0.f));
        if (AttackAnim.ElapsedTime > AttackAnim.TotalTime)
        {
            AttackAnim.Phase = Action;
            AttackAnim.TotalTime = 0.5f;
            AttackAnim.ElapsedTime = 0.f;

            auto player = owner->GetFrontObject(ObjectType::Player);
            _vec3 playerpos = player->GetComponent<TransformComponent>()->GetPosition();
            _vec3 selfpos = GetComponent<TransformComponent>()->GetPosition();
            _vec3 dir = playerpos - selfpos;
            dir.y = 0;
            Bones["Body"]->GetComponent<TransformComponent>()->SetForward(dir);
        }
        break;
    }
    case Action:
    {
        AttackAnim.ElapsedTime += dt;

        if (AttackAnim.ElapsedTime > AttackAnim.TotalTime)
        {
            _vec3 pos = GetComponent<TransformComponent>()->GetPosition();
            bullets[Index]->GetComponent<TransformComponent>()->SetPosition(pos);
            bullets[Index++]->SetActive(true);
            if (Index >= 10) Index = 0;
            AttackAnim.Phase = Recover;
            AttackAnim.TotalTime = 0.5f;
            AttackAnim.ElapsedTime = 0.f;

            EngineCore::GetInstance()->GetSoundManager()->PlaySFX("AttackShulker");
        }
        break;
    }
    case Recover:
    {
        AttackAnim.ElapsedTime += dt;

        _float t = clamp(AttackAnim.ElapsedTime / AttackAnim.TotalTime, 0.f, 1.f);
        _float pos = lerp(15.f, -2.f, t);
        _float Angle = lerp(90.f, 0.f, t);
        Bones["Head"]->GetComponent<TransformComponent>()->SetPosition(_vec3(0.f * Scale, pos * Scale, 0.f * Scale));
        Bones["Head"]->GetComponent<TransformComponent>()->SetRotate(_vec3(0.f, D3DXToRadian(Angle), 0.f));
        if (AttackAnim.ElapsedTime > AttackAnim.TotalTime)
        {
            AttackAnim.Phase = Ready;
            AttackAnim.ElapsedTime = 0.f;
            AttackAnim.TotalTime = 0.5f;
            AttackAnim.DelayTime = 5.f;
            *IsAttack = false;
        }
        break;
    }
    }
}

void Shulker::PlayDie(_float dt)
{
    DieAnim.ElapsedTime += dt;
    
    _float t = clamp(DieAnim.ElapsedTime / DieAnim.TotalTime, 0.f, 1.f);

    _float Angle = lerp(DieAnim.Start, DieAnim.End, t);

    GetComponent<TransformComponent>()->SetRotate({0.f, 0.f, -D3DXToRadian(Angle)});

    if (DieAnim.ElapsedTime > DieAnim.TotalTime)
    {
        DieAnim.IsEnd = true;
        DropEmeralds();
        SetDead();
        DeleteBar();
    }
}

void Shulker::Free()
{
    Monster::Free();
}
