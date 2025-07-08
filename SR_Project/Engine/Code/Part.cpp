#include "EnginePCH.h"
#include "Part.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

Part::Part(ObjectManager* owner, ObjectType objType) : Object(owner, objType)
{
}

Part::~Part()
{
}

Part* Part::Create(ObjectManager* owner, ObjectType objType, _vec3 scale, Object* parent, const std::string& mtrl)
{
    Part* Instance = new Part(owner, objType);

    if (FAILED(Instance->Ready_Object(parent, scale, mtrl)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT Part::Ready_Object(Object* Parent, _vec3 scale, const std::string& mtrl)
{
    Object::Ready_Object();

    auto transform = AddComponent<TransformComponent>();
    if (Parent != nullptr) transform->SetParent(Parent);
    transform->SetScale(scale);

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    renderer->SetMesh("Cube_Mesh");
    renderer->SetMaterial(mtrl);

    return S_OK;
}

void Part::Update(_float dt)
{
    Object::Update(dt);
}

void Part::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Part::Free()
{
    Object::Free();
}