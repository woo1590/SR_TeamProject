#include "pch.h"
#include "Blastling.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "FireBlock.h"
#include "ObjectManager.h"
#include "InfoComponent.h"
#include "IsTargetClose.h"
#include "BackStep.h"
#include "AIController.h"
#include "IsAlive.h"
#include "Rotate.h"
#include "Die.h"
#include "BehaviorTree.h"
#include "BlackBoard.h"
#include "IsTargetInAttackRange.h"
#include "Chase.h"
#include "Attack.h"
#include "SequenceNode.h"
#include "SelectorNode.h"
#include "BlastlingProjectile.h"
#include "Teleport.h"
#include "SoundManager.h"
#include "EngineCore.h"
#include "Material.h"
Blastling::Blastling(ObjectManager* owner, ObjectType objType)
	:Monster(owner, objType)
{
}

Blastling::~Blastling()
{
}

Blastling* Blastling::Create(ObjectManager* owner, ObjectType objType)
{
    Blastling* Instance = new Blastling(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Blastling::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Monster::Ready_Object(owner, objType);

    InitTransform(objType);

    InitTree();

    InitAnimation();
    InitProjectile();
    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(2.f, 20.f, 2.f));

	return S_OK;
}

void Blastling::Update(_float dt)
{
    Monster::Update(dt);
    PlayAnimation(dt);
}

void Blastling::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void Blastling::MoveTo(_vec3* dir, _float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    auto Stat = GetComponent<InfoComponent<EnemyInfo>>();

    if (State != MonsterState::Walk) State = MonsterState::Walk;
    D3DXVec3Normalize(dir, dir);
    Transform->Translate(*dir * dt * Stat->GetInfo().speed);
    Transform->SetForward(_vec3(dir->x, 0.f, dir->z));
}

void Blastling::RotateTo(_vec3* dir, float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    Transform->SetForward(_vec3(dir->x, 0.f, dir->z));
}

void Blastling::Attack(Object* target)
{
    if (State != MonsterState::Attack)
    {
        State = MonsterState::Attack;
        AttackAnim.DelayTime = 0.f;
        AttackAnim.ElapsedTime = 0;
        SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
        SetRotation({ 0.f, 0.f, 0.f }, "RLeg");

        AttackAnim.Phase = Ready;
        *IsAttack = true;
    }
}

void Blastling::Die()
{
    if (State != MonsterState::Die)
    {
        State = MonsterState::Die;
        DieAnim.ElapsedTime = 0;
        DieStartY = Bones["Body"]->GetComponent<TransformComponent>()->GetPosition().y;
        auto collision = GetComponent<CollisionComponent>();
        collision->SetSize(_vec3(0.1f, 0.1f, 0.1f));
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("DeathBlastling");
    }
}

void Blastling::Hit(_vec3 dir, _float power)
{
    if (State != MonsterState::Hit)
    {
        State = MonsterState::Hit;

        *IsHit = true;
        HitAnim.IsRunning = true;
        HitAnim.IsEnd = false;

        HitAnim.ElapsedTime = 0.f;
        HitAnim.DelayTime = 0.f;

        HitDir = dir;
        HitPower = power;

        SetRotation({ 0.f, 0.f, 0.f }, "LLeg");
        SetRotation({ 0.f, 0.f, 0.f }, "RLeg");
        Monster::Hit(dir, power);
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("HurtBlastling");

        for (auto& material : materials)
        {
            material->SetFloat("emissive", 1);
        }
    }
}

void Blastling::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);
    //transform->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
    
    SetMaterial("BlastlingHead_Mtrl", "Head");
    SetMaterial("BlastlingBody_Mtrl", "Body");
    SetMaterial("BlastlingLeg_Mtrl", "LLeg");
    SetMaterial("BlastlingLeg_Mtrl", "RLeg");
    SetMaterial("BlastlingArm_Mtrl", "LArm");
    SetMaterial("BlastlingArm_Mtrl", "RArm");

    materials.push_back(GetMaterial("Head"));
    materials.push_back(GetMaterial("Body"));
    materials.push_back(GetMaterial("LArm"));
    materials.push_back(GetMaterial("RArm"));
    materials.push_back(GetMaterial("LLeg"));
    materials.push_back(GetMaterial("RLeg"));

    for (auto& material : materials)
    {
        material->SetInt("coloruse", 0);
        material->SetVec3("color", _vec3(1.0, 0.0, 0.0));
        material->SetFloat("emissive", 0);
        material->SetVec3("emissivecolor", _vec3(0.5, 0.0, 0.0));
        material->SetFloat("emissivePow", 1);
    }

    auto headtransform = Bones["Head"]->GetComponent<TransformComponent>();
    headtransform->SetScale(_vec3(12.f * Scale, 12.f * Scale, 12.f * Scale));
    headtransform->SetPosition(_vec3(0.f, 20.f * Scale, 8.f * Scale));

    HeadFire = FireBlock::Create(owner, ObjectType::Bone);
    HeadFire->SetActive(true);
    HeadFire->GetComponent<CollisionComponent>()->SetSize(_vec3(0.f, 0.f, 0.f));
    HeadFire->GetComponent<TransformComponent>()->SetParent(Bones["Head"]);
    HeadFire->GetComponent<TransformComponent>()->SetScale(1.5f,1.f, 1.5f);
    HeadFire->GetComponent<TransformComponent>()->SetPosition(0.f,0.f, -0.5f);
    HeadFire->SetColor(_vec3(0.8f, 0.5f, 0.8f));
    owner->AddObject(ObjectType::Bone, HeadFire);

    auto bodytransofrm = Bones["Body"]->GetComponent<TransformComponent>();
    bodytransofrm->SetScale(_vec3(15.f * Scale, 20.f * Scale, 8.f * Scale));

    auto leftarmtransform = Bones["LArm"]->GetComponent<TransformComponent>();
    leftarmtransform->SetScale(_vec3(8.f * Scale, 60.f * Scale, 8.f * Scale));
    leftarmtransform->SetPosition(-23.f * Scale, -40.f* Scale, 0.f);
    leftarmtransform->SetPivot(_vec3(23.f * Scale, -8.f * Scale, 0.0f));

    auto rightarmtransform = Bones["RArm"]->GetComponent<TransformComponent>();
    rightarmtransform->SetScale(_vec3(8.f * Scale, 60.f * Scale, 8.f * Scale));
    rightarmtransform->SetPosition(23.f * Scale, -40.f * Scale, 0.f);
    rightarmtransform->SetPivot(_vec3(-23.f * Scale, -8.f * Scale, 0.0f));

    auto leftlegtransform = Bones["LLeg"]->GetComponent<TransformComponent>();
    leftlegtransform->SetScale(2.f * Scale, 40.f * Scale, 2.f * Scale);
    leftlegtransform->SetPosition(-6.f * Scale, -50.f * Scale, 0.f);
    leftlegtransform->SetPivot(_vec3(0.0f, 0.f * Scale, 0.0f));

    auto rightlegtransform = Bones["RLeg"]->GetComponent<TransformComponent>();
    rightlegtransform->SetScale(2.f * Scale, 40.f * Scale, 2.f * Scale);
    rightlegtransform->SetPosition(6.f * Scale, -50.f * Scale, 0.f);
    rightlegtransform->SetPivot(_vec3(0.0f, 0.f * Scale, 0.0f));
}

void Blastling::InitTree()
{
    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    bb->SetValue("Target", owner->GetObjectList(ObjectType::Player).back());
    Distance = new float(40.f);
    bb->SetValue("Distance", Distance);
    IsHit = new _bool(false);
    bb->SetValue("IsDamaged", IsHit);
    IsAttack = new _bool(false);
    bb->SetValue("IsAttack", IsAttack);
    NearDistance = new float(20.f);
    bb->SetValue("NearDistance", NearDistance);

    SequenceNode* rotateThenAttack = new SequenceNode();
    rotateThenAttack->AddChild(new RotateNode());
    rotateThenAttack->AddChild(new AttackNode());
    IsTargetInAttackRange* attackCheck = new IsTargetInAttackRange(rotateThenAttack);

    IsTargetClose* closecheck = new IsTargetClose(new TeleportNode());

    SelectorNode* attackBehavior = new SelectorNode();
    attackBehavior->AddChild(closecheck);
    attackBehavior->AddChild(attackCheck);
    attackBehavior->AddChild(new ChaseNode());

    IsAliveNode* isAlive = new IsAliveNode(attackBehavior);

    SelectorNode* root = new SelectorNode();
    root->AddChild(isAlive);
    root->AddChild(new DieNode());

    BehaviorTree* bt = BehaviorTree::Create(root);

    auto AI = AddComponent<AIController>(bt, bb);
}

void Blastling::InitAnimation()
{
    //Walk
    WalkAnim.ElapsedTime = 0.f;

    //Attack
    AttackAnim.ElapsedTime = 0.f;
    AttackAnim.TotalTime = 1.f;

    //Hit
    HitAnim.Start = 0;
    HitAnim.End = 30.f;
    HitAnim.ElapsedTime = 0.f;
    HitAnim.TotalTime = 0.3f;
    HitAnim.DelayTime = 0.f;
    HitAnim.Phase = Ready;

    //Die
    DieAnim.Start = 0;
    DieAnim.End = 90;
    DieAnim.ElapsedTime = 0.f;
    DieAnim.TotalTime = 0.7f;
}

void Blastling::PlayAnimation(_float dt)
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
        PlayHit(dt);
        break;
    case MonsterState::Die:
        if (!DieAnim.IsRunning) DieAnim.IsRunning = true;
        PlayDie(dt);
        break;
    }
}

void Blastling::PlayIdle(_float dt)
{
}

void Blastling::PlayWalk(_float dt)
{
    WalkAnim.ElapsedTime += dt;

    float MaxAngle = D3DXToRadian(20.f);
    float Angle = sinf(WalkAnim.ElapsedTime * Speed);

    SetRotation({ MaxAngle * Angle, 0.f, 0.f }, "LLeg");
    SetRotation({ -MaxAngle * Angle, 0.f, 0.f }, "RLeg");

    SetRotation({ -MaxAngle * Angle, 0.f, 0.f }, "LArm");
    SetRotation({ MaxAngle * Angle, 0.f, 0.f }, "RArm");
}

void Blastling::PlayAttack(_float dt)
{
    switch (AttackAnim.Phase)
    {
    case Ready:
    {
        AttackAnim.ElapsedTime += dt;

        float t = clamp(AttackAnim.ElapsedTime/AttackAnim.TotalTime, 0.f, 1.f);
        float leftangle = lerp(0.f, -105.f, t);
        float rightangle = leftangle + 15.f;
        
        leftangle = clamp(leftangle, -90.f, 0.f);

        SetRotation(_vec3(D3DXToRadian(leftangle), 0.f, 0.f), "LArm");
        SetRotation(_vec3(D3DXToRadian(rightangle), 0.f, 0.f), "RArm");

        if (AttackAnim.TotalTime < AttackAnim.ElapsedTime)
        {
            AttackAnim.Phase = Action;
            AttackAnim.ElapsedTime = 0.f;
            AttackAnim.TotalTime = 1.f;

            LeftAttack = false;
            RightAttack = false;
            EngineCore::GetInstance()->GetSoundManager()->PlaySFX("AttackBlastling");
        }
        break;
    }
    case Action:
    {
        if (AttackAnim.ElapsedTime > 0.f && !LeftAttack)
        {
            auto transform = GetComponent<TransformComponent>();
            _vec3 pos = transform->GetPosition();

            auto leftarmtransform = Bones["LArm"]->GetComponent<TransformComponent>();
            _vec3 leftpos = leftarmtransform->GetWorldPosition();
            _vec3 playerpos = owner->GetFrontObject(ObjectType::Player)->GetComponent<TransformComponent>()->GetPosition();

            _vec3 forward = transform->GetFoward();

            forward.y = playerpos.y - leftpos.y;
            Projectiles[ProjectileIndex]->GetComponent<TransformComponent>()->SetPosition(leftpos);
            Projectiles[ProjectileIndex]->FireProjectile(forward);
            Projectiles[ProjectileIndex++]->SetVisible(true);
            if (ProjectileIndex >= 10) ProjectileIndex = 0;
            LeftAttack = true;
        }

        if (AttackAnim.ElapsedTime > 0.5f && !RightAttack)
        {
            auto transform = GetComponent<TransformComponent>();
            _vec3 pos = transform->GetPosition();

            auto rightarmtransform = Bones["RArm"]->GetComponent<TransformComponent>();
            _vec3 rightpos = rightarmtransform->GetWorldPosition();

            _vec3 playerpos = owner->GetFrontObject(ObjectType::Player)->GetComponent<TransformComponent>()->GetPosition();

            _vec3 forward = transform->GetFoward();
            forward.y = playerpos.y - rightpos.y;

            Projectiles[ProjectileIndex]->GetComponent<TransformComponent>()->SetPosition(rightpos);
            Projectiles[ProjectileIndex]->FireProjectile(forward);
            Projectiles[ProjectileIndex++]->SetVisible(true);
            if (ProjectileIndex >= 10) ProjectileIndex = 0;
            RightAttack = true;
        }

        AttackAnim.ElapsedTime += dt;
        if (AttackAnim.TotalTime < AttackAnim.ElapsedTime)
        {
            AttackAnim.Phase = Recover;
            AttackAnim.ElapsedTime = 0.f;
            AttackAnim.TotalTime = 1.f;
        }
        break;
    }
    case Recover:
    {
        AttackAnim.ElapsedTime += dt;

        float t = clamp(AttackAnim.ElapsedTime / AttackAnim.TotalTime, 0.f, 1.f);
        float angle = lerp(-90.f, 0.f, t);

        SetRotation(_vec3(D3DXToRadian(angle), 0.f, 0.f), "LArm");
        SetRotation(_vec3(D3DXToRadian(angle), 0.f, 0.f), "RArm");
        if (AttackAnim.TotalTime < AttackAnim.ElapsedTime)
        {
            AttackAnim.Phase = Ready;
            AttackAnim.IsRunning = false;
            AttackAnim.IsEnd = true;
            AttackAnim.ElapsedTime = 0.f;
            AttackAnim.TotalTime = 1.f;
            *IsAttack = false;
        }
        break;
    }
    }
}

void Blastling::PlayDie(_float dt)
{
    DieAnim.ElapsedTime += dt;

    _float t = clamp(DieAnim.ElapsedTime / DieAnim.TotalTime, 0.f, 1.f);

    _float Angle = lerp(DieAnim.Start, DieAnim.End, t);

    SetRotation({ 0.f, 0.f, D3DXToRadian(Angle) });
    float yOffset = lerp(DieStartY, DieStartY - 6.f,t);

    auto transform = Bones["Body"]->GetComponent<TransformComponent>();
    _vec3 pos = transform->GetPosition();
    transform->SetPosition(_vec3(pos.x, yOffset, pos.z));

    if (DieAnim.ElapsedTime > DieAnim.TotalTime)
    {
        DieAnim.IsEnd = true;
        HeadFire->SetDead();
        DropEmeralds();
        SetDead();
        DeleteBar();
    }
}

void Blastling::PlayHit(_float dt)
{
    HitAnim.ElapsedTime += dt;

    if (HitAnim.ElapsedTime > 0.15)
    {
        for (auto& material : materials)
        {
            material->SetFloat("emissive", 0);
        }
    }

    _float t = clamp(HitAnim.ElapsedTime / HitAnim.TotalTime, 0.f, 1.f);

    _float Angle = lerp(HitAnim.Start, HitAnim.End, t);

    SetRotation({ -D3DXToRadian(Angle / 3), 0.f, 0.f }, "Head");
    SetRotation({ 0.f, 0.f, -D3DXToRadian(Angle/2) }, "LArm");
    SetRotation({ 0.f, 0.f, D3DXToRadian(Angle/2) }, "RArm");
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

void Blastling::OnCollisionStay(Object* other)
{
}

void Blastling::InitProjectile()
{
    for (int i = 0; i < 10; ++i)
    {
        auto projectile = BlastlingProjectile::Create(owner, ObjType);
        Projectiles.push_back(projectile);
        projectile->SetVisible(false);
    }
}

void Blastling::Free()
{
    Safe_Delete(NearDistance);
    Monster::Free();
}
