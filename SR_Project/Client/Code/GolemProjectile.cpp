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
    collision->SetSize(_vec3(0.5f, 0.5f, 0.5f));

    renderer->SetMesh("Cube_Mesh");
    renderer->SetMaterial("RedGolemLightblock_Mtrl");
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
        collision->SetSize(_vec3(0.5f, 0.5f, 0.5f));
        renderer->SetRenderID(RENDER_ID::Render_NonAlpha);
    }
}

void GolemProjectile::PlayScaleAnimation(_float dt)
{
    if (!IsOn) return;

    ElapsedTime += dt;

    if (ElapsedTime > 10.f) SetOn(false);

    auto transform = GetComponent<TransformComponent>();

    float Scale = (sinf(ElapsedTime * D3DX_PI) * 0.5f) + 0.1f;

    transform->SetScale(_vec3(Scale, Scale, Scale));

}

void GolemProjectile::Free()
{
    Object::Free();
}
