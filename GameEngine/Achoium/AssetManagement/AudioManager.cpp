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
            // ����ʹ�ü򵥵�Windows APIʵ��
            // ��ʵ����Ŀ�п����滻Ϊ���߼�����Ƶ����FMOD/OpenAL
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
        // ��ʼ����Ƶϵͳ
        ACMSG("Audio system initialized");
    }

    AudioManager::~AudioManager()
    {
        // ����������Ƶ��Դ
        StopAll();
        ACMSG("Audio system shutdown");
    }

    AudioID AudioManager::RegisterAudio(const std::string& name, const std::string& filePath)
    {
        // ��������Ƿ��Ѵ���
        auto it = nameToID.find(name);
        if (it != nameToID.end())
        {
            ACMSG("WARNING: Audio clip '" << name << "' already registered with ID: " << it->second);
            return it->second;
        }

        // �����µ�ID��ע��
        AudioID id = nextID++;
        nameToID[name] = id;
        audioClips[id] = AudioClip(id, name, filePath);
		audioClips[id].Load(); // ������Ƶ����
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

        // ����ʵ������
        float actualVolume = volume * masterVolume;
        
        // ʹ��Windows API������Ƶ
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

        // ʹ��Windows APIֹͣ��Ƶ����ǰ��ʵ�ֻ�ֹͣ����������
        // ע�⣺PlaySound�ĵ�ǰʵ���޷�ֹͣ�ض���Ƶ��ֻ��ֹͣ������Ƶ
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
        // Windows API�е�PlaySound��ֱ��֧����ͣ
        // ����ʵ��Ŀ�У�ʹ����FMOD�����Ŀ����ʵ�ִ˹���
    }

    void AudioManager::Resume(AudioID id)
    {
        // ͬ�ϣ��ڻ���Windows API�в�ֱ��֧��
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