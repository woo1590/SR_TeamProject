#include "pch.h"
#include "LaserHead.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "MeshRendererComponent.h"
#include "Bone.h"
#include "ObjectManager.h"
#include "Material.h"
#include "LaserEffect.h"

LaserHead::LaserHead(ObjectManager* owner, ObjectType objType)
	:Object(owner, objType)
{
}

LaserHead::~LaserHead()
{
}

LaserHead* LaserHead::Create(ObjectManager* owner, ObjectType objType)
{
    LaserHead* Instance = new LaserHead(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT LaserHead::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();

    auto collision = AddComponent<CollisionComponent>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetLayer(LAYER_ENEMY);
    collision->SetMask(LAYER_PLAYER | LAYER_DEFAULT);

    Bones["BottomHead"] = Bone::Create(owner, ObjectType::Bone, _vec3(1.f, 1.f, 1.f), this, "EnderBottomHead_Mtrl");
    Bones["TopHead"] = Bone::Create(owner, ObjectType::Bone, _vec3(1.f, 1.f, 1.f), Bones["BottomHead"], "EnderTopHead_Mtrl");
    Lasers.push_back(LaserEffect::Create(owner, ObjectType::ParticleEffect));
    Lasers.push_back(LaserEffect::Create(owner, ObjectType::ParticleEffect));

    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);
    physics->SetMass(1.f);

    _float Scale = 0.1f;
    auto toptransform = Bones["TopHead"]->GetComponent<TransformComponent>();
    toptransform->SetScale(24.f * Scale, 18.f * Scale, 24.f * Scale);
    toptransform->SetPosition(0.0f, 24.f * Scale, 0.f);
    toptransform->SetPivot(0.f, -18.f * Scale, 24.f * Scale);
    toptransform->SetPivotEnable(true);

    auto bottomtransform = Bones["BottomHead"]->GetComponent<TransformComponent>();
    bottomtransform->SetScale(24.f * Scale, 6.f * Scale, 24.f * Scale);

    for (auto& Laser : Lasers)
    {
        auto Lasertransform = Laser->GetComponent<TransformComponent>();
        Lasertransform->SetParent(Bones["BottomHead"]);
        Lasertransform->SetPosition(_vec3(0.f, 12.f * Scale, 48.f * Scale));
        Lasertransform->SetRotate(_vec3(0.f, D3DXToRadian(-90.f), 0.f));
    }

    Lasers[0]->GetComponent<TransformComponent>()->SetRotate(_vec3(0.f, D3DXToRadian(-90.f), 0.f));
    Lasers[1]->GetComponent<TransformComponent>()->SetRotate(_vec3(0.f, D3DXToRadian(90.f), 0.f));

    for (auto& bone : Bones)
        owner->AddObject(ObjectType::Bone, bone.second);
    for (auto& Laser : Lasers)
        owner->AddObject(ObjectType::ParticleEffect, Laser);

    Dir = HeadDir::Down;
    SetDir();
    State = HeadState::Open;
	return S_OK;
}

void LaserHead::Update(_float dt)
{
    Object::Update(dt);
    PlayAnimation(dt);
}

void LaserHead::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void LaserHead::SetActive(_bool Active)
{
    IsActive = Active;

    if (IsActive)
    {
        State = HeadState::Open;
    }
    else
    {
        State = HeadState::Close;
    }
}

void LaserHead::InitAnimation()
{
    StartAnim.Start = 0.f;
    StartAnim.End = -45.f;
    StartAnim.TotalTime = 0.7f;
    StartAnim.ElapsedTime = 0.f;

    EndAnim.Start = -45.f;
    EndAnim.End = 0.f;
    EndAnim.TotalTime = 0.7f;
    EndAnim.ElapsedTime = 0.f;
}

void LaserHead::PlayAnimation(_float dt)
{
    switch (State)
    {
    case HeadState::Open:
        PlayOpen(dt);
        break;
    case HeadState::Idle:
        break;
    case HeadState::Close:
        PlayClose(dt);
        break;
    }
}

void LaserHead::PlayOpen(_float dt)
{
    StartAnim.ElapsedTime += dt;
    
    _float t = clamp(StartAnim.ElapsedTime / StartAnim.TotalTime, 0.f, 1.f);
    
    float angle = lerp(StartAnim.Start, StartAnim.End, t);

    auto toptransform = Bones["TopHead"]->GetComponent<TransformComponent>();
    toptransform->SetRotate(D3DXToRadian(angle), 0.f, 0.f);

    if (StartAnim.ElapsedTime > StartAnim.TotalTime)
        State = HeadState::Idle;
}

void LaserHead::PlayClose(_float dt)
{
}

void LaserHead::SetDir()
{
    auto transform = GetComponent<TransformComponent>();
    switch (Dir)
    {
    case HeadDir::Left:
        transform->SetRotate(0.f, D3DXToRadian(-90.f), 0.f);
        break;
    case HeadDir::Right:
        transform->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
        break;
    case HeadDir::Up:
        transform->SetRotate(0.f, 0.f, 0.f);
        break;
    case HeadDir::Down:
        transform->SetRotate(0.f, D3DXToRadian(180.f), 0.f);
        break;
    }
}

void LaserHead::Free()
{
    Object::Free();
}
