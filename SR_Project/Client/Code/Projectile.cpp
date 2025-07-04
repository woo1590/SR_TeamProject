#include "pch.h"
#include "Projectile.h"

#include "TransformComponent.h"
#include "ObjectManager.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"

Projectile::Projectile(ObjectManager* owner, ObjectType objType) : Object(owner, objType) {}

Projectile::~Projectile() {}

void Projectile::Free()
{
	Object::Free();
}
Projectile* Projectile::Create(ObjectManager* owner, ObjectType objType)
{
    Projectile* Instance = new Projectile(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT Projectile::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    if (FAILED(Object::Ready_Object()))
        return E_FAIL;

    auto transform = AddComponent<TransformComponent>();
    auto info = AddComponent<InfoComponent<ProjectileInfo>>();
    auto mesh = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    owner->AddObject(objType, this);

    return S_OK;
}

void Projectile::Update(_float dt)
{
    Object::Update(dt);
}

void Projectile::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

