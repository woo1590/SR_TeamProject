#pragma once

class Loader :
    public Base
{
private:
    Loader(LOADID loadId);
    virtual ~Loader();

public:
    static Loader* Create(LOADID loadId);
    HRESULT Ready_Loader();
    HRESULT Loading();
    _bool IsFinished()const;
    static _uint __stdcall LoaderMain(void* arg);
private:
    HRESULT Load_Village();
    HRESULT Load_Stage1();
    HRESULT load_UIResources();

    void Free()override;
    
    HANDLE H_Thread;
    CRITICAL_SECTION Crt;
    _bool Is_Finish = false;
    LOADID LoadId;
};

