#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

struct Particle
{
    _vec3 position;
    _vec3 velocity;
    _float size;
    _float life;
    _bool isActive = false;
};

class ENGINE_DLL ParticleSystem :
    public ObjectComponent
{
private:
    ParticleSystem(Object* owner);
    virtual ~ParticleSystem();
    void Update(_float dt)override;

public:
    static ParticleSystem* Create(Object* owner);

    std::vector<Particle> activePool;
    std::vector<Particle> deactivePool;
};

END