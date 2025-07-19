#include "pch.h"
#include "GolemProjectile.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "ObjectManager.h"
#include "CollisionComponent.h"
#include "EngineCore.h"
#include "CollisionSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "PhysicsComponent.h"
#include "Material.h"
#include "InfoComponent.h"
#include "Player.h"
#include "Circle.h"
#include "PhysicsSystem.h"

GolemProjectile::GolemProjectile(ObjectManager* owner, ObjectType objType)
	:Object(owner, objType)
{
}

GolemProjectile::~GolemProjectile()
{
}

GolemProjectile* GolemProjectile::Create(ObjectManager* owner, ObjectType objType)
{
    GolemProjectile* Instance = new GolemProjectile(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT GolemProjectile::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    
    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<AABBCollider>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetLayer(LAYER_PROJECTILE);
    collision->SetMask(LAYER_PLAYER);
    collision->SetCollisionEnter([this](Object* other) {this->OnCollisionStay(other); });
    collision->SetSize(_vec3(5.f, 5.f, 5.f));

    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);
    physics->SetKinematic(true);
    physics->SetMass(1.f);

    renderer->SetMesh("Cube_Mesh");
    renderer->SetMaterial("RedGolemLightblock_Mtrl");

    auto mtrl = renderer->GetMaterial();
    mtrl->SetInt("emissive", 1);

    mtrl->SetInt("coloruse", 1);
    mtrl->SetVec3("color", _vec3(1.0, 0.0, 0.0));
    mtrl->SetFloat("emissive", 1);
    mtrl->SetVec3("emissivecolor", _vec3(1.0, 0.1, 0));
    mtrl->SetFloat("emissivePow", 3);
    circle = Circle::Create(owner, ObjectType::Projectile, this);

    SetOn(false);
    owner->AddObject(objType, this);

    return S_OK;
}

void GolemProjectile::Update(_float dt)
{
    Object::Update(dt);
    PlayScaleAnimation(dt);
}

void GolemProjectile::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void GolemProjectile::SetOn(_bool On)
{
    IsOn = On;

    auto renderer = GetComponent<MeshRenderer>();
    auto collision = GetComponent<CollisionComponent>();

    if (!IsOn)
    {
        ElapsedTime = 0.f;
        collision->SetSize(_vec3(0.f, 0.f, 0.f));
        renderer->SetRenderID(RENDER_ID::Render_None);
    }
    else
    {
        collision->SetSize(_vec3(5.f, 5.f, 5.f));
        renderer->SetRenderID(RENDER_ID::Render_Alpha);
    }

    static_cast<Circle*>(circle)->SetOn(IsOn);
}

void GolemProjectile::PlayScaleAnimation(_float dt)
{
    if (!IsOn) return;

    ElapsedTime += dt;

    if (ElapsedTime > 10.f) SetOn(false);

    auto transform = GetComponent<TransformComponent>();

    float Scale = (sinf(ElapsedTime * D3DX_PI) * 0.5f) + 0.3f;

    transform->SetScale(_vec3(Scale, Scale, Scale));

}

void GolemProjectile::OnCollisionStay(Object* other)
{
    ObjectType objType = other->GetObjectType();

    if (objType == ObjectType::Player)
    {
        auto playerStat = other->GetComponent<InfoComponent<PlayerInfo>>();
        playerStat->AddHp(-1);
    }
}

void GolemProjectile::Free()
{
    Object::Free();
}
