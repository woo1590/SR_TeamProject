#include "ThirdcamComponent.h"
#include "TransformComponent.h"

ThirdcamComponent::ThirdcamComponent(Object* owner)
    :ObjectComponent(owner)
{
}

ThirdcamComponent::~ThirdcamComponent()
{
}

ThirdcamComponent* ThirdcamComponent::Create(Object* owner)
{
    ThirdcamComponent* Instance = new ThirdcamComponent(owner);

    return Instance;
}

void ThirdcamComponent::Update(_float dt)
{
    ObjectComponent::Update(dt);


}

void ThirdcamComponent::SetTarget(TransformComponent* target)
{
    Target = target;
}

void ThirdcamComponent::Free()
{
    ObjectComponent::Free();
}
