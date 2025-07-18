#pragma once
#include "Monster.h"

enum class EnderState {CrawlToStand, StandToCrawl, Crawl, Stand, Hidden, Sprout, CrossLaser, LineLaser, Projectile};

class LaserEffect;
class LaserHead;
class EnderProjectile;
class Ender :
    public Monster
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
    void        Crawl();
    void        Stand();
    void        Hide();
    void        Sprout();
    void        CrawlToStand();
    void        StandToCrawl();
    void        LineLaserAttack();
    void        CrossLaserAttack();
    void        ProjectileAttack();
    void        MoveTo(_vec3 targetPos, _float dt);

    void        SetState(EnderState state);
    EnderState  GetState();
    int         GetCurChangeStateCount();

private:
    void        InitLaserHead();
    void        InitCrossLaser();
    void        InitEnderProjectile();

    void        PlayCrawl(_float dt);
    void        PlayCrawlToStand(_float dt);
    void        PlayStandToCrawl(_float dt);
    void        PlayStand(_float dt);
    void        PlayHide(_float dt);
    void        PlaySprout(_float dt);
    void        PlayLineLaserAttack(_float dt);
    void        PlayCrossLaserAttack(_float dt);
    void        PlayProjectileAttack(_float dt);

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

    vector<LaserHead*>  LaserHeads;
    float               LaserSpawnTime = 0.f;
    _int                LaserIndex = 0;

    vector<LaserEffect*>        CrossLasers;

    vector<EnderProjectile*>    EnderProjectiles;
    float                       ProjectileSpawnTime = 0.f;
    _int                        ProjectileIndex = 0;
};

