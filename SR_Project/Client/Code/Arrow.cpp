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
