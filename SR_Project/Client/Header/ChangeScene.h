#pragma once
#include "ICommand.h"

class ChangeScene :
    public ICommand
{
private:
    ChangeScene(LOADID nextSceneID);
    virtual ~ChangeScene();

public:
    static ChangeScene* Create(LOADID nextSceneID);
    void Execute()override;

private:
    LOADID nextSceneID;
};

