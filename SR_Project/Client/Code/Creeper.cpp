#include "pch.h"
#include "Creeper.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Chase.h"
#include "Attack.h"
#include "SequenceNode.h"
#include "SelectorNode.h"
#include "BehaviorTree.h"
#include "BlackBoard.h"
#include "AIController.h"
#include "IsAlive.h"
#include "Die.h"
#include "IsTargetInAttackRange.h"
#include "ObjectManager.h"
#include "InfoComponent.h"
#include "Material.h"

#include "Scene.h"
#include "ThirdcamComponent.h"
#include "CameraManager.h"
#include "CameraComponent.h"
#include "EngineCore.h"
#include "SoundManager.h"
#include "ExplodeEffect.h"

Creeper::Creeper(ObjectManager* owner, ObjectType objType)
    :Monster(owner, objType)
{
}

Creeper::~Creeper()
{
}

Creeper* Creeper::Create(ObjectManager* owner, ObjectType objType)
{
    Creeper* Instance = new Creeper(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Creeper::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Monster::Ready_Object(owner, objType, MonsterType::Creeper);

    //InitTransform
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);
    InitTransform(objType);

    //Init Collision
    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(2.f, 5.5f, 2.f));

    //Create BT
    InitTree();

    //Animation
    InitAnimation();
    return S_OK;
}

void Creeper::Update(_float dt)
{
    Monster::Update(dt);
    PlayAnimation(dt);
}

void Creeper::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void Creeper::MoveTo(_vec3* dir, _float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    auto Stat = GetComponent<InfoComponent<EnemyInfo>>();

    if (State != MonsterState::Walk) State = MonsterState::Walk;
    D3DXVec3Normalize(dir, dir);

    if (DieAnim.ElapsedTime < 5.f)
    {
        Transform->Translate(*dir * dt * Stat->GetInfo().speed * 0.6);
        Transform->SetForward(_vec3(dir->x, 0.f, dir->z));
    }
    if (DieAnim.ElapsedTime < 2.f)
    {
        emissiveOn = false;
        SetEmissive(emissiveOn);
    }
}

void Creeper::RotateTo(_vec3* dir, float dt)
{
}

void Creeper::Die()
{
    if (State != MonsterState::Die)
    {
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("DeathCreeper");
        State = MonsterState::Die;

        DieAnim.IsRunning = true;
        DieAnim.IsEnd = false;
        DieAnim.DelayTime = 0.0f;
        emissiveOn = false;

        if (DieAnim.ElapsedTime < 1.f) DieAnim.ElapsedTime = 0.0f;
    }
}

void Creeper::InitTransform(ObjectType objType)
{
    auto transform = GetComponent<TransformComponent>();
    transform->SetPosition(20.f, 100.f, 20.f);

    SetMaterial("CreeperFace_Mtrl", "Head");
    SetMaterial("CreeperBody_Mtrl", "Body");
    SetMaterial("CreeperLeg_Mtrl", "LArm");
    SetMaterial("CreeperLeg_Mtrl", "RArm");
    SetMaterial("CreeperLeg_Mtrl", "LLeg");
    SetMaterial("CreeperLeg_Mtrl", "RLeg");

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
        material->SetFloat("emissive", 1);
        material->SetVec3("emissivecolor", _vec3(1.0, 0.1, 0.03));
        material->SetFloat("emissivePow", 2);
    }
    //body
    SetScale(_vec3(7.f * Scale, 16.f * Scale, 4.f * Scale), "Body");

    //arm
    SetScale(_vec3(4.f * Scale, 5.f * Scale, 4.f * Scale), "LArm");               //front
    SetPosition(_vec3(-6.f * Scale, -20.f * Scale, 5.5f * Scale), "LArm");       //front
    Bones["LArm"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 10.f * Scale, 0.0f));
    SetScale(_vec3(4.f * Scale, 5.f * Scale, 4.f * Scale), "RArm");               //front
    SetPosition(_vec3(6.f * Scale, -20.f * Scale, 5.5f * Scale), "RArm");        //front
    Bones["RArm"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 10.f * Scale, 0.0f));

    //leg
    SetScale(_vec3(4.f * Scale, 5.f * Scale, 4.f * Scale), "LLeg");               //back
    SetPosition(_vec3(-6.f * Scale, -20.f * Scale, -5.5f * Scale), "LLeg");      //back
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 10.f * Scale, 0.0f));
    SetScale(_vec3(4.f * Scale, 5.f * Scale, 4.f * Scale), "RLeg");               //back
    SetPosition(_vec3(6.f * Scale, -20.f * Scale, -5.5f * Scale), "RLeg");       //back
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 10.f * Scale, 0.0f));
}

void Creeper::InitTree()
{
    IsTargetInAttackRange* attackCheck = new IsTargetInAttackRange(new DieNode());

    SequenceNode* attackSequence = new SequenceNode();
    attackSequence->AddChild(attackCheck);

    SelectorNode* BehaviorNode = new SelectorNode();
    BehaviorNode->AddChild(attackSequence);
    BehaviorNode->AddChild(new ChaseNode());

    IsAliveNode* IsAlive = new IsAliveNode(BehaviorNode);

    SelectorNode* root = new SelectorNode();
    root->AddChild(IsAlive);

    BehaviorTree* bt = BehaviorTree::Create(root);

    BlackBoard* bb = BlackBoard::Create();
    bb->SetValue("Self", this);
    bb->SetValue("Target", owner->GetObjectList(ObjectType::Player).back());
    Distance = new float(3.f);
    bb->SetValue("Distance", Distance);

    auto AI = AddComponent<AIController>(bt, bb);
}

void Creeper::InitAnimation()
{
    WalkAnim.ElapsedTime = 0.5f;

    DieAnim.ElapsedTime = 0.f;
    DieAnim.DelayTime = 0.f;
}

void Creeper::PlayAnimation(_float dt)
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

void Creeper::PlayIdle(_float dt)
{
}

void Creeper::PlayWalk(_float dt)
{
    WalkAnim.ElapsedTime += dt;

    float Angle = sinf(WalkAnim.ElapsedTime * 7.f);
    SetRotation({ Angle/ 2, 0.f, 0.f }, "LLeg");
    SetRotation({ -Angle / 2, 0.f, 0.f }, "RLeg");

    SetRotation({ Angle / 2, 0.f, 0.f }, "LArm");
    SetRotation({ -Angle / 2, 0.f, 0.f }, "RArm");

    if (DieAnim.ElapsedTime > 2.f)
    {
        PlayDie(dt);
    }
}

void Creeper::PlayAttack(_float dt)
{
}

void Creeper::PlayDie(_float dt)
{
    DieAnim.ElapsedTime += dt;
    DieAnim.DelayTime -= dt;

    float blinkInterval = max(0.05f, 0.5f - DieAnim.ElapsedTime * 0.1f);

    if (DieAnim.DelayTime <= 0.0f)
    {
        emissiveOn = !emissiveOn;
        SetEmissive(emissiveOn);
        DieAnim.DelayTime = blinkInterval;
    }

    if (DieAnim.ElapsedTime >= 6.f && !DieAnim.IsEnd)
    {
        DieAnim.IsEnd = true;

        auto player = owner->GetObjectList(ObjectType::Player).back();
        _vec3 playerpos = player->GetComponent<TransformComponent>()->GetPosition();
        _vec3 pos = GetComponent<TransformComponent>()->GetPosition();

        _vec3 length = playerpos - pos;
        if (D3DXVec3Length(&length) < 10)
            player->GetComponent<InfoComponent<PlayerInfo>>()->AddHp(-15);
        
        SetDead();
        ///////////////////////////////Shake Cam
        auto cam = GetScene()->GetCameraManager()->GetMainCamera()->
                   GetOwner()->GetComponent<ThirdcamComponent>();
        if (cam)
            cam->SetShake(8.f, 0.4f);

        auto effect = ExplodeEffect::Create(owner, ObjectType::ParticleEffect);
        effect->GetComponent<TransformComponent>()->SetPosition(GetComponent<TransformComponent>()->GetPosition());
        effect->SetDeadTime(1.f);
        owner->AddObject(ObjectType::ParticleEffect, effect);

        ///////////////////////////////
    }
}

void Creeper::OnCollisionStay(Object* other)
{
}

void Creeper::SetEmissive(_bool On)
{
    if (emissiveOn)
    {
        for (auto& material : materials)
            material->SetFloat("emissive", 1.f);
    }
    else
    {
        for (auto& material : materials)
            material->SetFloat("emissive", 0);
    }
}

void Creeper::Free()
{
    Monster::Free();
}
