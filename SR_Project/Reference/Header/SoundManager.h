#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL SoundManager
    : public Base
{
private:
    SoundManager();
    virtual ~SoundManager(){}

public:
    static SoundManager* Create();
    HRESULT Ready_SoundManager();
    void Update();

    void LoadSound(const std::string& key, const TCHAR* filepath, bool loop = false);
    void PlaySFX(const std::string& key);
    void PlayBGM(const std::string& key);
    void Stop(const std::string& key);

    void SetSFXVolume(float volume);  // 0.0f ~ 1.0f
    void SetBGMVolume(float volume);

    void MuteSFX(bool mute);
    void MuteBGM(bool mute);

    // 추후 확장용
    void FadeOut(const std::string& key, float duration);
private:
    void Free()override;

    FMOD::System* m_System = nullptr;
    FMOD::ChannelGroup* m_SFXGroup = nullptr;
    FMOD::ChannelGroup* m_BGMGroup = nullptr;

    std::unordered_map<std::string, FMOD::Sound*> m_SoundMap;
    std::unordered_map<std::string, FMOD::Channel*> m_ChannelMap;
};

END