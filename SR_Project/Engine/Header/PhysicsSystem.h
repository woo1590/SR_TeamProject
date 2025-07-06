#pragma once
#include "Base.h"

BEGIN(Engine)

class Scene;
class CollisionComponent;
class PhysicsComponent;
class StaticGrid;
class ENGINE_DLL PhysicsSystem :
    public Base
{
    typedef struct CollisionPair
    {
        CollisionComponent* a;
        CollisionComponent* b;

        CollisionPair(CollisionComponent* a, CollisionComponent* b)
        {
            if (a < b)
            {
                this->a = a;
                this->b = b;
            }
            else
            {
                this->a = b;
                this->b = a;
            }
        }

        bool operator<(const CollisionPair& other) const
        {
            if (a != other.a)
                return a < other.a;
            else
                return b < other.b;
        }
    };

    typedef struct AABBEntry
    {
        CollisionComponent* comp;
        _float minX, maxX;
    };
private:
    PhysicsSystem(Scene* owner);
    virtual ~PhysicsSystem();

    static constexpr float GRAVITY = 30.f; 
public:
    static PhysicsSystem* Create(Scene* owner);
    HRESULT Ready_PhysicsSystem();
    void Update(_float dt);

    void RegisterBody(PhysicsComponent* body);

private:
    void ApplyGravity(_float dt);
    void ApplyVelocity(_float dt);
    void CollectBlocks();
    void BroadPhase();
    void SolvePosition();
    void CollisionEvent();

    void Free()override;
    std::vector<PhysicsComponent*> DynamicBodies;
    std::vector<AABBEntry> AABBEntries;
    StaticGrid* Grid = nullptr;

    std::set<CollisionPair> CurrCollisions;
    std::set<CollisionPair> PrevCollisions;
    Scene* owner = nullptr;
};

END