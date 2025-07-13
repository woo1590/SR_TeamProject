#pragma once
#include "ObjectComponent.h"
#include "EngineCore.h"
#include "Random.h"

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
    _float ratePerSec = 0.f;
    _bool looping = false;
    _bool followCam = false;

    _float acc = 0.f;
    _bool alive = true;

    /*-------Particle param-------*/

    _vec4 color{ 1.f,1.f,1.f,1.f };
    _vec4 colorFade{ 0.f,0.f,0.f,0.f };
    _vec3 velocityMin{ 0.f,0.f,0.f };
    _vec3 velocityMax{ 0.f,0.f,0.f };
    _vec3 spawnAreaMin;
    _vec3 spawnAreaMax;
    _float size = 1.f;
    _float life = 1.f;

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

            InitParticle(p, spawnPos);

            --want;
        }
    }
    
    void InitParticle(Particle& p, _vec3 pos)
    {
        auto r = EngineCore::GetInstance()->GetRandom();

        p.color = color;
        p.colorFade = colorFade;
        p.age = 0.f;
        p.size = size;
        p.life = life;

        _float spawnX = r->get<_float>(pos.x - spawnAreaMin.x, pos.x + spawnAreaMax.x);
        _float spawnY = r->get<_float>(pos.y - spawnAreaMin.y, pos.y + spawnAreaMax.y);
        _float spawnZ = r->get<_float>(pos.z - spawnAreaMin.z, pos.z + spawnAreaMax.z);
        p.position = _vec3(spawnX, spawnY, spawnZ);

        _float velocityX = r->get<_float>(velocityMin.x, velocityMax.x);
        _float velocityY = r->get<_float>(velocityMin.y, velocityMax.y);
        _float velocityZ = r->get<_float>(velocityMin.z, velocityMax.z);
        p.velocity = _vec3(velocityX, velocityY, velocityZ);

        p.isActive = true;
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