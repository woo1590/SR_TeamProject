#include "pch.h"
#include "Circle.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "ObjectManager.h"
#include "Material.h"

Circle::Circle(ObjectManager* owner, ObjectType objType)
	:Object(owner, objType)
{
}

Circle::~Circle()
{
}

Circle* Circle::Create(ObjectManager* owner, ObjectType objType, Object* parent)
{
    Circle* Instance = new Circle(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType, parent)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT Circle::Ready_Object(ObjectManager* owner, ObjectType objType, Object* parent)
{
    auto transform = AddComponent<TransformComponent>();
    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_None);
    transform->SetScale(_vec3(5.f, 0.1f, 5.f));

    renderer->SetMesh("Cube_Mesh");
    renderer->SetMaterial("Circle_Mtrl");

    if (parent != nullptr)
        transform->SetParent(parent);
    
    owner->AddObject(ObjectType::Projectile, this);

    auto mtrl = renderer->GetMaterial();
    mtrl->SetInt("emissive", 1);
    mtrl->SetVec3("color", _vec3(1.0, 0.1, 0.0));
    mtrl->SetFloat("emissive", 1);
    mtrl->SetVec3("emissivecolor", _vec3(1.0, 0.0, 0));
    mtrl->SetFloat("emissivePow", 5);
    return S_OK;
}

void Circle::Update(_float dt)
{
    Object::Update(dt);
}

void Circle::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Circle::SetOn(_bool On)
{
    auto renderer = GetComponent<MeshRenderer>();
    if (On)
    {
        renderer->SetRenderID(RENDER_ID::Render_Alpha);
    }
    else
    {
        renderer->SetRenderID(RENDER_ID::Render_None);
    }
}

void Circle::Free()
{
    Object::Free();
}
