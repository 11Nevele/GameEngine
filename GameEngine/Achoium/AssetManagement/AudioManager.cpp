#include "acpch.h"
#include "AudioManager.h"
#include <Windows.h>
#include <mmsystem.h>
#include "Debug.h"
#pragma comment(lib, "winmm.lib")

namespace ac
{
    void AudioClip::Load()
    {
        if (!loaded && !filePath.empty())
        {
            // 这里使用简单的Windows API实现
            // 在实际项目中可以替换为更高级的音频库如FMOD/OpenAL
            loaded = true;
        }
    }

    void AudioClip::Unload()
    {
        if (loaded && data)
        {
            free(data);
            data = nullptr;
            size = 0;
            loaded = false;
        }
    }

    AudioManager::AudioManager() : nextID(1)
    {
        // 初始化音频系统
        ACMSG("Audio system initialized");
    }

    AudioManager::~AudioManager()
    {
        // 清理所有音频资源
        StopAll();
        ACMSG("Audio system shutdown");
    }

    AudioID AudioManager::RegisterAudio(const std::string& name, const std::string& filePath)
    {
        // 检查名称是否已存在
        auto it = nameToID.find(name);
        if (it != nameToID.end())
        {
            ACMSG("WARNING: Audio clip '" << name << "' already registered with ID: " << it->second);
            return it->second;
        }

        // 分配新的ID并注册
        AudioID id = nextID++;
        nameToID[name] = id;
        audioClips[id] = AudioClip(id, name, filePath);
		audioClips[id].Load(); // 加载音频数据
        ACMSG("Registered audio clip '" << name << "' with ID: " << id);
        return id;
    }

    AudioID AudioManager::GetAudioID(const std::string& name) const
    {
        auto it = nameToID.find(name);
        if (it != nameToID.end())
        {
            return it->second;
        }
        return INVALID_AUDIO_ID;
    }

    AudioClip* AudioManager::GetAudioClip(AudioID id)
    {
        auto it = audioClips.find(id);
        if (it != audioClips.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    AudioClip* AudioManager::GetAudioClipByName(const std::string& name)
    {
        AudioID id = GetAudioID(name);
        if (id != INVALID_AUDIO_ID)
        {
            return GetAudioClip(id);
        }
        return nullptr;
    }

    bool AudioManager::Play(AudioID id, bool loop, float volume)
    {
        if (muted || id == INVALID_AUDIO_ID) return false;

        auto it = audioClips.find(id);
        if (it == audioClips.end())
        {
            ACMSG("WARNING: Audio clip with ID " << id << " not found.");
            return false;
        }

        // 计算实际音量
        float actualVolume = volume * masterVolume;
        
        // 使用Windows API播放音频
        DWORD flags = SND_FILENAME | SND_ASYNC;
        if (loop) flags |= SND_LOOP;
        
        return PlaySoundA(it->second.filePath.c_str(), NULL, flags);
    }

    bool AudioManager::PlayByName(const std::string& name, bool loop, float volume)
    {
        AudioID id = GetAudioID(name);
        if (id != INVALID_AUDIO_ID)
        {
            return Play(id, loop, volume);
        }
        
        ACMSG("WARNING: Audio clip '" << name << "' not registered.");
        return false;
    }

    void AudioManager::Stop(AudioID id)
    {
        if (id == INVALID_AUDIO_ID) return;

        // 使用Windows API停止音频（当前简化实现会停止所有声音）
        // 注意：PlaySound的当前实现无法停止特定音频，只能停止所有音频
        PlaySoundA(NULL, NULL, 0);
    }

    void AudioManager::StopByName(const std::string& name)
    {
        AudioID id = GetAudioID(name);
        if (id != INVALID_AUDIO_ID)
        {
            Stop(id);
        }
    }

    void AudioManager::Pause(AudioID id)
    {
        // Windows API中的PlaySound不直接支持暂停
        // 在真实项目中，使用像FMOD这样的库可以实现此功能
    }

    void AudioManager::Resume(AudioID id)
    {
        // 同上，在基本Windows API中不直接支持
    }

    void AudioManager::StopAll()
    {
        PlaySoundA(NULL, NULL, 0);
    }

    void AudioManager::SetMasterVolume(float volume)
    {
        masterVolume = std::clamp(volume, 0.0f, 1.0f);
    }

    void AudioManager::SetMuted(bool isMuted)
    {
        muted = isMuted;
        if (muted)
        {
            StopAll();
        }
    }
}