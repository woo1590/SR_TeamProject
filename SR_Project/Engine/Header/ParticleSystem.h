#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

struct Particle
{
    _vec4 color;
    _vec4 colorFade;
    _vec3 position;
    _vec3 velocity;
    _float size;
    _float life;
    _float age;
    _bool isActive = false;
};

struct Emitter
{
    _uint burstCount = 0;
    _float ratePerSec;
    _bool looping = false;
    _bool followCam = false;

    _float timer = 0.f;
    _bool alive = true;

    _vec3 spawnPos;
    _vec3 spawnAreaMin;
    _vec3 spawnAreaMax;

    void Spawn(std::vector<Particle>& particles, _float dt)
    {

    }
};


class ENGINE_DLL ParticleSystem :
    public ObjectComponent
{
    using UpdateFunc = std::function<void(Particle& p, _float dt)>;

private:
    ParticleSystem(Object* owner);
    virtual ~ParticleSystem();

public:
    static ParticleSystem* Create(Object* owner);
    HRESULT Ready_Component()override;
    void Update(_float dt)override;

    const std::vector<Particle>& GetParticles()const { return particles; }
    void AddEmitter(Emitter emitter, UpdateFunc update);
private:
    void Free()override;

    static constexpr int MAX_PARTICLES = 1024;
    static constexpr float GRAVITY = 9.8f;

    std::vector<Particle> particles;
    Emitter emitter;
    UpdateFunc particleUpdate;
};

END