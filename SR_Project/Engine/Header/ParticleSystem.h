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

    _float acc = 0.f;
    _bool alive = true;

    _vec3 spawnPos;
    _vec3 spawnAreaMin;
    _vec3 spawnAreaMax;

    /*-------Particle param-------*/

    _vec4 color;
    _vec4 colorFade;
    _vec3 position;
    _vec3 velocityMin;
    _vec3 velocityMax;
    _float size;
    _float life;

    void Spawn(std::vector<Particle>& particles, _vec3 spawnPos, _float dt)
    {
        if (!alive) return;

        int want = 0;

        if (burstCount > 0)
        {
            want = burstCount;

            burstCount = 0;
            alive = false;
        }

        if (ratePerSec > 0 && looping)
        {
            acc += ratePerSec * dt;
            int n = static_cast<_uint>(acc);

            if (n)
            {
                want += n;
                acc -= n;
            }
        }

        for (auto& p : particles)
        {
            if (!want)
                break;

            if (p.isActive)
                continue;


            --want;
        }
    }
    
    void InitParitcle(Particle& p, _vec3 pos)
    {
        p.color = color;
        p.colorFade = colorFade;
        p.position = pos;
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