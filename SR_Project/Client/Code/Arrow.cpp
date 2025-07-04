#include "pch.h"
#include "Arrow.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"
#include "CollisionComponent.h"

Arrow::Arrow(ObjectManager* owner, ObjectType objType) : Projectile(owner, objType) {}
Arrow::~Arrow() {}
void Arrow::Free()
{
	Projectile::Free();
}

Arrow* Arrow::Create(ObjectManager* owner, ObjectType objType, ObjectType shooterType, _vec3 normalDirection)
{
    Arrow* Instance = new Arrow(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType, shooterType, normalDirection)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT Arrow::Ready_Object(ObjectManager* owner, ObjectType objType, ObjectType shooterType, _vec3 normalDirection)
{
    if (FAILED(Projectile::Ready_Object(owner,objType)))
        return E_FAIL;

    auto info = GetComponent<InfoComponent<ProjectileInfo>>();
    ProjectileInfo i;
    i.speed = 30.f;
    i.shooterType = shooterType;
    i.size = 1.f;
    i.attackDamage = 10.f;
    i.scale = _vec3{ 0.1f, 1.f, 1.f };
    i.direction = normalDirection;
    i.meshType = L"Cube_Mesh";
    i.material = L"arrow_Mtrl";
    i.renderId = Engine::RENDER_ID::Render_Alpha;
    info->SetInfo(i);

    auto transform = GetComponent<TransformComponent>();
    transform->SetScale(i.scale.x * i.size, i.scale.y * i.size, i.scale.z * i.size);

    auto mesh = GetComponent<MeshRenderer>();
    mesh->SetMesh(i.meshType);
    mesh->SetMaterial(i.material);
    mesh->SetRenderID(i.renderId);

    auto collision = AddComponent<CollisionComponent>();
    collision->SetLayer(CollisionComponent::LAYER_PLAYER);
    collision->SetMask(CollisionComponent::LAYER_ENEMY);
    collision->SetSize(_vec3(1.f, 2.f, 1.f));
    collision->SetCollisionEnter([this](Object* other) {this->SetCollisionEnter(other); });
    return S_OK;
}

void Arrow::Update(_float dt)
{
    Projectile::Update(dt);

    auto transform = GetComponent<TransformComponent>();
    auto info = GetComponent<InfoComponent<ProjectileInfo>>()->GetInfo();
    transform->Translate(info.direction * dt);
}

void Arrow::Late_Update(_float dt)
{
    Projectile::Late_Update(dt);
}

void Arrow::SetCollisionEnter(Object* other)
{
    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();

    if (objType == ObjectType::Monster) {
        float arrowAttackDamage = GetComponent<InfoComponent<ProjectileInfo>>()->GetInfo().attackDamage;
        other->GetComponent<InfoComponent<EnemyInfo>>()->AddHp(-arrowAttackDamage);
        collision->ResolveAABBColiision(other);
    }
}
