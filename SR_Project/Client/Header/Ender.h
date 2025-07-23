#pragma once
#include "Boss.h"

enum class EnderState {CrawlToStand, StandToCrawl, Crawl, Stand, Hidden, HideIdle, Sprout, CrossLaser, LineLaser, Projectile, Die};

class LaserEffect;
class LaserHead;
class EnderProjectile;
class FireBlock;
class Ender :
    public Boss
{
protected:
    Ender(ObjectManager* owner, ObjectType objType);
    virtual ~Ender();

public:
    static Ender* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

protected:
    void InitTransform(ObjectType objType);
    void InitTree();
    void InitAnimation() override;
    void PlayAnimation(_float dt) override;

protected:
    void Free() override;

public:
    void        SetTargetPos(_vec3 pos);
    void        Crawl();
    void        Stand();
    void        Hide();
    void        HideIdle();
    void        Sprout();
    void        CrawlToStand();
    void        StandToCrawl();
    void        LineLaserAttack();
    void        CrossLaserAttack();
    void        ProjectileAttack();
    void        MoveTo(_vec3 targetPos, _float dt);
    void        Die() override;

    void        SetState(EnderState state);
    EnderState  GetState();
    int         GetCurChangeStateCount();

private:
    void        InitFireBlock();
    void        InitLaserHead();
    void        InitCrossLaser();
    void        InitEnderProjectile();

    void        PlayCrawl(_float dt);
    void        PlayCrawlToStand(_float dt);
    void        PlayStandToCrawl(_float dt);
    void        PlayStand(_float dt);
    void        PlayHide(_float dt);
    void        PlayHideIdle(_float dt);
    void        PlaySprout(_float dt);
    void        PlayLineLaserAttack(_float dt);
    void        PlayCrossLaserAttack(_float dt);
    void        PlayProjectileAttack(_float dt);
    void        PlayDie(_float dt);

    void        SetBoneSize();
    
    _float      EasedT(_float t);

private:
    Animation   CrawlAnim;
    Animation   CrawlToStandAnim;
    Animation   StandToCrawlAnim;
    Animation   StandAnim;
    Animation   HideAnim;
    Animation   SproutAnim;

    Animation   LaserAttackAnim;
    Animation   HeadAttackAnim;
    Animation   ProjectileAttackAnim;

    EnderState  enderState;

    _float      HideStartY;
    int         CurChangeStateCount;
    int*        ChangeStateCount = nullptr;
    _vec3*      TargetPos = nullptr;
    _bool*      IsDie = nullptr;

    _float              FireSpawnTime = 0.f;

    _float              LaserSpawnTime = 0.f;

    vector<LaserEffect*>        CrossLasers;
    
    vector<EnderProjectile*>    EnderProjectiles;
    _float                      ProjectileSpawnTime = 0.f;
    _int                        ProjectileIndex = 0;

    _float                      SproutSoundDelay = 0.f;
    _bool                       IsSproutPlay = false;

    _float                      CrossLaserSoundTime = 0.f;
    _float                      WalkTime = 0.f;

    _bool                       deadEffect = false;
};

