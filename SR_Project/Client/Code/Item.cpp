#include "pch.h"
#include "Item.h"
#include "Scene.h"
#include "CollisionSystem.h"

#include "TransformComponent.h"
#include "ObjectManager.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"
#include "CollisionComponent.h"

Item::Item(ObjectManager* owner, ObjectType objType) :Object(owner, objType) {}
Item::~Item() {}

void Item::Free()
{
    Object::Free();
}
Item* Item::Create(ObjectManager* owner, ObjectType objType)
{
    Item* Instance = new Item(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT Item::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    if (FAILED(Object::Ready_Object()))
        return E_FAIL;

    auto transform = AddComponent<TransformComponent>();
    auto info = AddComponent<InfoComponent<ItemInfo>>();
    auto mesh = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);

    owner->AddObject(objType, this);
    
    return S_OK;
}

void Item::Update(_float dt)
{
    Object::Update(dt);
}

void Item::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Item::SetScale(_float _scale)
{
    itemScale = _scale;
}

void Item::SetScaleRatio(_vec3 _scaleRatio)
{
    itemScaleRatio = _scaleRatio;
}

void Item::SetPosition(_vec3 _position)
{
    itemPosition = _position;
}

void Item::SetPivot(bool _pivotEnable, _vec3 _pivotPosition)
{
    itemPivotEnable = _pivotEnable;
    pivotPosition = _pivotPosition;
}

void Item::SetRotation(_vec3 _rotation)
{
    itemRotation = _rotation;
}

void Item::SetRenderId(Engine::RENDER_ID _renderId)
{
    renderId = _renderId;
}

void Item::SetOwnerObject(Object* _ownerObject)
{
    ownerObject = _ownerObject;
}

void Item::SetMesh(std::string _meshType)
{
    meshType = _meshType;
}

void Item::SetMaterial(std::string _material)
{
    material = _material;
}

void Item::ApplyComponents()
{
    auto transform = GetComponent<TransformComponent>();
    transform->SetScale(itemScaleRatio.x * itemScale, itemScaleRatio.y * itemScale, itemScaleRatio.z * itemScale);
    transform->SetPosition(itemPosition.x * itemScale, itemPosition.y * itemScale, itemPosition.z * itemScale);
    transform->SetPivot(_vec3(pivotPosition.x * itemScale, pivotPosition.y * itemScale, pivotPosition.z * itemScale));
    transform->SetPivotEnable(itemPivotEnable);
    transform->SetRotate({ itemRotation.x ,itemRotation.y ,itemRotation.z });

    auto mesh = GetComponent<MeshRenderer>();
    mesh->SetMesh(meshType);
    mesh->SetMaterial(material);
    mesh->SetRenderID(renderId);
}

Item::ItemType Item::GetItemType()
{
    return itemType;
}

