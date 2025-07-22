#include "pch.h"
#include "Pig.h"
#include "Bone.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "CollisionComponent.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "Scene.h"

Pig::Pig(ObjectManager* owner, ObjectType objType)
    :Object(owner, objType)
{
}

Pig::~Pig()
{
}

Pig* Pig::Create(ObjectManager* owner, ObjectType objType)
{
    Pig* Instance = new Pig(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Pig::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Object::Ready_Object();

    InitTransform();
    InitCollision();
    InitPhysics();

    return S_OK;
}

void Pig::Update(_float dt)
{
    Object::Update(dt);
    MoveTo(dt);
    PlayAnimation(dt);
}

void Pig::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Pig::InitTransform()
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetRotate(0.f, D3DXToRadian(90.f), 0.f);

    Bones["Body"] = Bone::Create(owner, ObjectType::Bone, _vec3(1.f * Scale, 1.f * Scale, 2.f * Scale), this, "PigBody_Mtrl");
    Bones["Head"] = Bone::Create(owner, ObjectType::Bone, _vec3(1.f * Scale, 1.f * Scale, 1.f * Scale), Bones["Body"], "PigFace_Mtrl");
    
    auto headtransform = Bones["Head"]->GetComponent<TransformComponent>();
    headtransform->SetPosition(0.f, 1.5f * Scale, 2.f * Scale);
    Bones["Nose"] = Bone::Create(owner, ObjectType::Bone, _vec3(0.4f * Scale, 0.2f* Scale, 0.1f* Scale), Bones["Head"], "PigNose_Mtrl");
    auto nosetransform = Bones["Nose"]->GetComponent<TransformComponent>();
    nosetransform->SetPosition(0.f, 0.f, 1.1f * Scale);

    Bones["LFLeg"] = Bone::Create(owner, ObjectType::Bone, _vec3(0.2 * Scale, 0.2 * Scale, 0.2 * Scale), Bones["Body"], "PigFoot_Mtrl");
    auto lflegtransform = Bones["LFLeg"]->GetComponent<TransformComponent>();
    lflegtransform->SetPosition(-1.f*Scale, -1.f*Scale, 1.5f * Scale);
    lflegtransform->SetPivot(1.f * Scale, 1.f * Scale, -1.5f * Scale);
    lflegtransform->SetPivotEnable(true);

    Bones["RFLeg"] = Bone::Create(owner, ObjectType::Bone, _vec3(0.2 * Scale, 0.2 * Scale, 0.2 * Scale), Bones["Body"], "PigFoot_Mtrl");
    auto rflegtransform = Bones["RFLeg"]->GetComponent<TransformComponent>();
    rflegtransform->SetPosition(1.f * Scale, -1.f * Scale, 1.5f * Scale);
    rflegtransform->SetPivot(-1.f * Scale, 1.f * Scale, -1.5f * Scale);
    rflegtransform->SetPivotEnable(true);

    Bones["LBLeg"] = Bone::Create(owner, ObjectType::Bone, _vec3(0.2 * Scale, 0.2 * Scale, 0.2 * Scale), Bones["Body"], "PigFoot_Mtrl");
    auto lblegtransform = Bones["LBLeg"]->GetComponent<TransformComponent>();
    lblegtransform->SetPosition(-1.f * Scale, -1.f * Scale, -1.5f * Scale);
    lblegtransform->SetPivot(1.f* Scale, 1.f * Scale, 1.5f * Scale);
    lblegtransform->SetPivotEnable(true);

    Bones["RBLeg"] = Bone::Create(owner, ObjectType::Bone, _vec3(0.2 * Scale, 0.2 * Scale, 0.2 * Scale), Bones["Body"], "PigFoot_Mtrl");
    auto rblegtransform = Bones["RBLeg"]->GetComponent<TransformComponent>();
    rblegtransform->SetPosition(1.f * Scale, -1.f * Scale, -1.5f * Scale);
    rblegtransform->SetPivot(-1.f * Scale, 1.f * Scale, 1.5f * Scale);
    rblegtransform->SetPivotEnable(true);

    for (auto& bone : Bones)
        owner->AddObject(ObjectType::Bone, bone.second);
}

void Pig::InitCollision()
{
    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<AABBCollider>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetSize(_vec3(3.f, 3.f, 3.f));
    collision->SetLayer(LAYER_NPC);
    collision->SetMask(LAYER_PLAYER | LAYER_DEFAULT);
}

void Pig::InitPhysics()
{
    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);//test
    physics->SetMass(1.f);
    physics->SetKinematic(true);
}

void Pig::MoveTo(_float dt)
{
    auto transform = GetComponent<TransformComponent>();
    _vec3 selfpos = transform->GetPosition();

    auto playertransform = owner->GetFrontObject(ObjectType::Player)->GetComponent<TransformComponent>();
    _vec3 playerpos = playertransform->GetPosition();

    _vec3 dir = playerpos - selfpos;

    if (D3DXVec3Length(&dir) >= 5)
    {
        if (D3DXVec3Length(&dir) >= 30) transform->SetPosition(playerpos.x - 5, playerpos.y, playerpos.z + 5);
        D3DXVec3Normalize(&dir, &dir);
        transform->Translate(dir.x * dt * 15.f, 0.f, dir.z * dt * 15.f);
    }
    _vec3 pos = transform->GetPosition();
    if (pos.y < playerpos.y + 5.f)
    {
        transform->SetPosition(pos.x, pos.y + 8.f * dt, pos.z);
    }

    D3DXVec3Normalize(&dir, &dir);
    transform->SetForward(_vec3(dir.x, 0.f, dir.z));
}

void Pig::PlayAnimation(_float dt)
{
    ElapsedTime += dt;
    float t = sinf(ElapsedTime * 10.f);
    _float maxAngle = 20.f;
    _float HeadAngle = maxAngle * t;
    Bones["LFLeg"]->GetComponent<TransformComponent>()->SetRotate(D3DXToRadian(HeadAngle * 2), 0, 0);
    Bones["RFLeg"]->GetComponent<TransformComponent>()->SetRotate(D3DXToRadian(-HeadAngle * 2), 0, 0);
    Bones["LBLeg"]->GetComponent<TransformComponent>()->SetRotate(D3DXToRadian(-HeadAngle * 2), 0, 0);
    Bones["RBLeg"]->GetComponent<TransformComponent>()->SetRotate(D3DXToRadian(HeadAngle * 2), 0, 0);

    Bones["Head"]->GetComponent<TransformComponent>()->SetRotate(D3DXToRadian(HeadAngle / 2), 0, 0);
}

void Pig::Free()
{
    Object::Free();
}
