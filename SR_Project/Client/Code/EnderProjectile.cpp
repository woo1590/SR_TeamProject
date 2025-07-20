#include "pch.h"
#include "EnderProjectile.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "Material.h"
#include "ObjectManager.h"
#include "FireBlock.h"

EnderProjectile::EnderProjectile(ObjectManager* owner, ObjectType objType)
    :Object(owner, objType)
{
}

EnderProjectile::~EnderProjectile()
{
}

EnderProjectile* EnderProjectile::Create(ObjectManager* owner, ObjectType objType)
{
    EnderProjectile* Instance = new EnderProjectile(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT EnderProjectile::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);

    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<AABBCollider>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetLayer(LAYER_PROJECTILE);
    collision->SetMask(LAYER_DEFAULT);
    collision->SetSize(_vec3(3.f, 3.f, 3.f));

    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);
    physics->SetMass(1.f);

    renderer->SetMesh("Cube_Mesh");
    renderer->SetMaterial("EnderProjectile_Mtrl");

    auto mtrl = renderer->GetMaterial();

    mtrl->SetInt("coloruse", 1);
    mtrl->SetFloat("emissive", 1);
    mtrl->SetVec3("emissivecolor", _vec3(1.0, 0.7137, 0.7568));
    mtrl->SetVec3("color", _vec3(0.3, 0.2, 0.25));
    mtrl->SetFloat("emissivePow", 0.9f);

    owner->AddObject(objType, this);

    ElapsedTime = 0.f;

    return S_OK;
}

void EnderProjectile::Update(_float dt)
{
    Object::Update(dt);
    PlayScaleAnimation(dt);
    CheckGround();
}

void EnderProjectile::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void EnderProjectile::PlayScaleAnimation(_float dt)
{
    if (ElapsedTime > 10.f)
        return;

    ElapsedTime += dt;

    float t = clamp(ElapsedTime / 10.f, 0.f, 1.f);

    float size = lerp(0.f, 1.f, t);
    auto transform = GetComponent<TransformComponent>();
    transform->SetScale(size, size, size);
}

void EnderProjectile::CheckGround()
{
    auto physics = GetComponent<PhysicsComponent>();
    if (physics->IsGrounded() && Visible)
    {
        physics->SetVelocity(_vec3(0.f, 0.f, 0.f));

        auto transform = GetComponent<TransformComponent>();
        _vec3 pos = transform->GetPosition();

        for (int i = 0; i < (rand() % 4 + 2); ++i)
        {
            int randx = rand() % 10 - 5;
            int randz = rand() % 10 - 5;

            _vec3 randpos = _vec3(randx + pos.x, pos.y, randz + pos.z);
            auto fire = FireBlock::Create(owner, ObjectType::SpriteEffect);
            fire->GetComponent<TransformComponent>()->SetPosition(randpos);
            fire->SetDeadTime(5.f);
            fire->SetColor(_vec3(0.8f, 0.5f, 0.8f));
            owner->AddObject(ObjectType::SpriteEffect, fire);
            SetVisible(false);
        }
    }
}

void EnderProjectile::Free()
{
    Object::Free();
}

void EnderProjectile::SetVisible(_bool visible)
{
    auto renderer = GetComponent<MeshRenderer>();
    Visible = visible;
    if (visible)
    {
        renderer->SetRenderID(RENDER_ID::Render_NonAlpha);
    }
    else
    {
        renderer->SetRenderID(RENDER_ID::Render_None);
    }
}

void EnderProjectile::FireProjectile(_vec3 dir)
{
    auto physics = GetComponent<PhysicsComponent>();
    int randvelocity = rand() % 10 + 10;
    physics->SetVelocity(_vec3(dir.x, randvelocity, dir.z));
}