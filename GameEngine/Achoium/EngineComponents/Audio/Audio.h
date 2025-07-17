#pragma once
#include "acpch.h"
#include "AssetManagement/AudioManager.h"
#include "Debug.h"
#include "Core/ECSEvents.h"
#include "Core/World.hpp"

namespace ac
{
    // ǰ������
    class World;
    
    // ��Ƶ����ö��
    enum class AudioType
    {
        Sound,    // ����Ч
        Music     // ��������
    };

    // ��Ƶ��� - ���ӵ���Ҫ����������ʵ����
    struct AudioSource
    {
        AudioID audioID = INVALID_AUDIO_ID;  // ������ԴID
        bool isPlaying = false;              // ��ǰ�Ƿ����ڲ���
        bool loop = false;                   // �Ƿ�ѭ������
        float volume = 1.0f;                 // ���� (0.0 - 1.0)
        float pitch = 1.0f;                  // ����
        AudioType type = AudioType::Sound;   // ��Ƶ����
        bool playOnStart = false;            // ʵ�崴��ʱ�Զ�����

        AudioSource() = default;
        static AudioSource Create(World& world, const std::string& name, 
            bool looping = false, float vol = 1.0f, AudioType audioType = AudioType::Sound, bool autoPlay = false)
        {
            AudioManager& audioManager = world.GetResourse<AudioManager>();
            AudioID id = audioManager.GetAudioID(name);
            if (id == INVALID_AUDIO_ID)
            {
                ACMSG("WARNING: Audio clip '" << name << "' not registered.");
                return AudioSource();
            }
            return AudioSource(id, looping, vol, audioType, autoPlay);
		}
        
        // ʹ��ID����
        AudioSource(AudioID id, bool looping = false, float vol = 1.0f, 
            AudioType audioType = AudioType::Sound, bool autoPlay = false)
            : audioID(id), loop(looping), volume(vol), type(audioType), playOnStart(autoPlay) {}
    };

    // ��������� - ͨ�����ӵ���������ʵ����
    struct AudioListener
    {
        bool active = true;

        AudioListener() = default;
        AudioListener(bool isActive) : active(isActive) {}
    };

    // �¼��ص���������
    bool OnAudioSourceAdded(const OnAdded<AudioSource>& event);
    bool OnAudioSourceDeleted(const OnDeleted<AudioSource>& event);
}