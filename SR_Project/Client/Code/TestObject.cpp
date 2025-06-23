#include "pch.h"
#include "TestObject.h"
#include "EngineCore.h"
#include "RenderSystem.h"

//component
#include "FreecamComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

TestObject::TestObject(ObjectManager* owner, ObjectType objType)
    :Object(owner,objType)
{
}

TestObject::~TestObject()
{
}

TestObject* TestObject::Create(ObjectManager* owner, ObjectType objType)
{
    TestObject* Instance = new TestObject(owner, objType);

    if (FAILED(Instance->Ready_Object()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT TestObject::Ready_Object()
{
    Object::Ready_Object();

    auto transform = AddComponent<TransformComponent>();

    auto fCam = AddComponent<FreecamComponent>();
    fCam->SetTarget(transform);

    auto cam = AddComponent<CameraComponent>();
    cam->SetTarget(transform);

    EngineCore::GetInstance()->GetRenderSystem()->SetCamera(cam);

    return S_OK;
}

void TestObject::Update(_float dt)
{
    Object::Update(dt);
}

void TestObject::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void TestObject::Free()
{
    Object::Free();
}
