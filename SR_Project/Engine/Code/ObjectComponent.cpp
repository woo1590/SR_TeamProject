#include "EnginePCH.h"
#include "ObjectComponent.h"
#include "Object.h"

ObjectComponent::ObjectComponent(Object* owner)
    :owner(owner)
{

}

ObjectComponent::~ObjectComponent()
{
}

HRESULT ObjectComponent::Ready_Component()
{
    return S_OK;
}

void ObjectComponent::Free()
{

}
