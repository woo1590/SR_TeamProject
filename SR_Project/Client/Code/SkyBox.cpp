#include "pch.h"
#include "SkyBox.h"


//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

SkyBox::SkyBox(ObjectManager* owner, ObjectType objType)
    :Object(owner,objType)
{
}

SkyBox::~SkyBox()
{
}

SkyBox* SkyBox::Create(ObjectManager* owner, ObjectType objType)
{
    SkyBox* Instance = new SkyBox(owner, objType);

    if (FAILED(Instance->Ready_Object()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT SkyBox::Ready_Object()
{
    auto transform = AddComponent<TransformComponent>();
    transform->SetScale(100.f, 100.f, 100.f);

    auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Priority);
    renderer->SetMesh(L"SkyBox_Mesh");
    renderer->SetMaterial(L"SkyBox_Material");

    return S_OK;
}

void SkyBox::Update(_float dt)
{
    Object::Update(dt);
}

void SkyBox::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void SkyBox::Free()
{
    Object::Free();
}
