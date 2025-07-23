#pragma once
#include "Object.h"

enum class HeadDir {Left, Right, Up, Down,};
enum class HeadState{Open, Idle, Close,};
struct HeadAnimation
{
    _float Start;       //start angle;
    _float End;         //end angle;

    _float TotalTime;   //total time
    _float ElapsedTime; //elapsed time
};

class LaserHead :
    public Object
{
protected:
    LaserHead(ObjectManager* owner, ObjectType objType);
    virtual ~LaserHead();

public:
    static LaserHead* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void SetActive(_bool Active);
    void SetDir(HeadDir dir);

    _bool IsActive()const { return isActive; }
private:
    void InitAnimation();
    void PlayAnimation(_float dt);
    void PlayOpen(_float dt);
    void PlayIdle(_float dt);
    void PlayClose(_float dt);


protected:
    void Free() override;

private:
    _float          ElapsedTime = 0.f;
    _bool           isActive;
    unordered_map<string, Object*>  Bones;

    vector<Object*>                 Lasers;
    HeadDir                         Dir;
    HeadState                       State;

    HeadAnimation                   StartAnim;
    HeadAnimation                   IdleAnim;
    HeadAnimation                   EndAnim;
};

