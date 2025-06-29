#include "pch.h"
#include "TestObject.h"
#include "EngineCore.h"
#include "RenderSystem.h"
#include "InputSystem.h"

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


    return S_OK;
}

void TestObject::Update(_float dt)
{
    Object::Update(dt);

    auto Input = EngineCore::GetInstance()->GetInputSystem();
    auto transform = GetComponent<TransformComponent>();

    if (Input->IsKeyDown(W))
        transform->Translate(0.f, 0.f, 100.f * dt);
    if (Input->IsKeyDown(A))
        transform->Translate(-100.f*dt, 0.f, 0.f);
    if (Input->IsKeyDown(S))
        transform->Translate(0.f, 0.f, -100.f * dt);
    if (Input->IsKeyDown(D))
        transform->Translate(100.f * dt, 0.f, 0.f);

    if (Input->IsKeyDown(SPACE))
        transform->Translate(0.f, 100.f * dt, 0.f);
}

void TestObject::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void TestObject::Free()
{
    Object::Free();
}
