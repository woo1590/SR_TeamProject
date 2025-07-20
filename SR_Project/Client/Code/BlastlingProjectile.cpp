#include "pch.h"
#include "BlastlingProjectile.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Scene.h"
#include "PhysicsComponent.h"
#include "MeshRendererComponent.h"
#include "PhysicsSystem.h"
#include "Material.h"
#include "ObjectManager.h"
#include "InfoComponent.h"

BlastlingProjectile::BlastlingProjectile(ObjectManager* owner, ObjectType objType)
    :Object(owner, objType)
{
}

BlastlingProjectile::~BlastlingProjectile()
{
}

BlastlingProjectile* BlastlingProjectile::Create(ObjectManager* owner, ObjectType objType)
{
    BlastlingProjectile* Instance = new BlastlingProjectile(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT BlastlingProjectile::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetScale(0.5f, 0.5f, 0.5f);
    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);

    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<AABBCollider>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetLayer(LAYER_PROJECTILE);
    collision->SetMask(LAYER_DEFAULT | LAYER_PLAYER);
    collision->SetCollisionEnter([this](Object* other) {this->OnCollisionEnter(other); });
    collision->SetSize(_vec3(1.f, 1.f, 1.f));

    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);
    physics->SetMass(1.f);
    physics->SetKinematic(true);

    renderer->SetMesh("Cube_Mesh");
    renderer->SetMaterial("EnderProjectile_Mtrl");

    auto mtrl = renderer->GetMaterial();

    mtrl->SetInt("coloruse", 1);
    mtrl->SetFloat("emissive", 1);
    mtrl->SetVec3("emissivecolor", _vec3(0.5, 0.0, 1.0));
    mtrl->SetVec3("color", _vec3(0.3, 0.2, 0.25));
    mtrl->SetFloat("emissivePow", 0.9f);

    owner->AddObject(objType, this);

    ElapsedTime = 0.f;

    return S_OK;
}

void BlastlingProjectile::Update(_float dt)
{
    Object::Update(dt);
    CheckGround();
    UpdateFire(dt);
}

void BlastlingProjectile::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void BlastlingProjectile::CheckGround()
{
    auto physics = GetComponent<PhysicsComponent>();
    if (physics->IsGrounded() && Visible)
    {
        physics->SetVelocity(_vec3(0.f, 0.f, 0.f));
        SetVisible(false);
    }
}

void BlastlingProjectile::OnCollisionEnter(Object* other)
{
    ObjectType objType = other->GetObjectType();

    if (objType == ObjectType::Player)
    {
        auto playerStat = other->GetComponent<InfoComponent<PlayerInfo>>();
        playerStat->AddHp(-30);

        {
            ElapsedTime = 0.f;
            Fire = false;
            SetVisible(false);
        }
    }
}

void BlastlingProjectile::UpdateFire(_float dt)
{
    if (!Fire) return;
    ElapsedTime += dt;
    auto transform = GetComponent<TransformComponent>();
    transform->Translate(FireDir.x * dt * 25.f, FireDir.y * dt * 0.5f, FireDir.z * dt * 25.f);

    if (ElapsedTime > 10.f)
    {
        ElapsedTime = 0.f;
        Fire = false;
        SetVisible(false);
    }
}

void BlastlingProjectile::Free()
{
    Object::Free();
}

void BlastlingProjectile::SetVisible(_bool visible)
{
    auto renderer = GetComponent<MeshRenderer>();
    auto collision = GetComponent<CollisionComponent>();

    Visible = visible;
    if (visible)
    {
        renderer->SetRenderID(RENDER_ID::Render_NonAlpha);
        collision->SetSize(_vec3(3.f, 3.f, 3.f));
    }
    else
    {
        renderer->SetRenderID(RENDER_ID::Render_None);
        collision->SetSize(_vec3(0.f, 0.f, 0.f));
    }
}

void BlastlingProjectile::FireProjectile(_vec3 dir)
{
    Fire = true;
    FireDir = dir;
}
