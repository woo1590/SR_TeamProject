#pragma once
#include "Base.h"

BEGIN(Engine)

class Scene;
class CollisionComponent;
class PhysicsComponent;
class ENGINE_DLL PhysicsSystem :
    public Base
{
private:
    PhysicsSystem(Scene* owner);
    virtual ~PhysicsSystem();

    static constexpr float GRAVITY = 30.f; 
public:
    static PhysicsSystem* Create(Scene* owner);
    HRESULT Ready_PhysicsSystem();
    void Update(_float dt);

    void RegisterBody(PhysicsComponent* body);
    void RegisterCollision(CollisionComponent* collision);

private:
    void ApplyGravity(_float dt);
    void ApplyVelocity(_float dt);
    void BroadPhase();
    void SolvePosition();
    void CollisionEvent();

    void Free()override;
    std::vector<CollisionComponent*> Collisions;
    std::vector<PhysicsComponent*> Bodies;

    std::vector<std::pair<CollisionComponent*, CollisionComponent*>> CurrCollision;
    std::vector<std::pair<CollisionComponent*, CollisionComponent*>> PrevCollision;
    Scene* owner = nullptr;
};

END