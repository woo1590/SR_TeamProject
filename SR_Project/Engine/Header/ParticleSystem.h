#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class Particle
{
    
};

class ENGINE_DLL ParticleSystem :
    public ObjectComponent
{
private:
    ParticleSystem(Object* owner);
    virtual ~ParticleSystem();

public:
    static ParticleSystem* Create(Object* owner);

};

END