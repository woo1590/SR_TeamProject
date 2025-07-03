#pragma once
#include "Base.h"

BEGIN(Engine)

class Scene;
class CollisionComponent;
class ENGINE_DLL PhysicsSystem :
    public Base
{
private:
    PhysicsSystem(Scene* owner);
    //virtual ~PhysicsSystem();

public:
    //static PhysicsSystem* Create(Scene* owner);
    //HRESULT Ready_PhysicsSystem();
    //
    //void ApplyGravity();
    //void CollisionCheck();

private:

};

END