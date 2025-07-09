#include "pch.h"
#include "Bone.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "Material.h"
#include "ResourceManager.h"
Bone::Bone(ObjectManager* owner, ObjectType objType)
	:Object(owner,objType)
{
}

Bone::~Bone()
{
}

Bone* Bone::Create(ObjectManager* owner, ObjectType objType, _vec3 scale, Object* parent, const std::string& mtrl)
{
    Bone* Instance = new Bone(owner, ObjectType::Bone);

    if (FAILED(Instance->Ready_Object(parent, scale, mtrl)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
  
}

HRESULT Bone::Ready_Object(Object* Parent, _vec3 scale, const std::string& mtrl)
{
    Object::Ready_Object();

    auto transform = AddComponent<TransformComponent>();
    if(Parent != nullptr) transform->SetParent(Parent);
    transform->SetScale(scale);

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
    renderer->SetMesh("Cube_Mesh");
    renderer->SetMaterial(mtrl);
    return S_OK;
}

void Bone::Update(_float dt)
{
    Object::Update(dt);
}

void Bone::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Bone::Free()
{
    Object::Free();
}
