#pragma once
#include "acpch.h"
#include "AssetManagement/AudioManager.h"
#include "Debug.h"
#include "Core/ECSEvents.h"
#include "Core/World.hpp"



//soloud
#include <soloud.h>
#include <soloud_audiosource.h>
#include <soloud_ay.h>
#include <soloud_bassboostfilter.h>
#include <soloud_biquadresonantfilter.h>
#include <soloud_bus.h>
#include <soloud_c.h>
#include <soloud_dcremovalfilter.h>
#include <soloud_duckfilter.h>
#include <soloud_echofilter.h>
#include <soloud_error.h>
#include <soloud_fader.h>
#include <soloud_fft.h>
#include <soloud_fftfilter.h>
#include <soloud_file.h>
#include <soloud_filter.h>
#include <soloud_flangerfilter.h>
#include <soloud_freeverbfilter.h>
#include <soloud_lofifilter.h>
#include <soloud_monotone.h>
#include <soloud_noise.h>
#include <soloud_openmpt.h>
#include <soloud_queue.h>
#include <soloud_robotizefilter.h>
#include <soloud_sfxr.h>
#include <soloud_speech.h>
#include <soloud_tedsid.h>
#include <soloud_thread.h>
#include <soloud_vic.h>
#include <soloud_vizsn.h>
#include <soloud_wav.h>
#include <soloud_waveshaperfilter.h>
#include <soloud_wavstream.h>

namespace ac
{
    // 前向声明
    class World;
    
    // 音频类型枚举
    enum class AudioType
    {
        Sound,    // 短音效
        Music     // 背景音乐
    };

    // 音频组件 - 附加到需要播放声音的实体上
    struct AudioSource
    {
        AudioID audioID = INVALID_AUDIO_ID;  // 声音资源ID
        bool isPlaying = false;              // 当前是否正在播放
        bool loop = false;                   // 是否循环播放
        float volume = 1.0f;                 // 音量 (0.0 - 1.0)
        float pitch = 1.0f;                  // 音高
        AudioType type = AudioType::Sound;   // 音频类型
        bool playOnStart = false;            // 实体创建时自动播放

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
        
        // 使用ID构造
        AudioSource(AudioID id, bool looping = false, float vol = 1.0f, 
            AudioType audioType = AudioType::Sound, bool autoPlay = false)
            : audioID(id), loop(looping), volume(vol), type(audioType), playOnStart(autoPlay) {}
    };

    // 监听器组件 - 通常附加到相机或玩家实体上
    struct AudioListener
    {
        bool active = true;

        AudioListener() = default;
        AudioListener(bool isActive) : active(isActive) {}
    };

    // 事件回调函数声明
    bool OnAudioSourceAdded(const OnAdded<AudioSource>& event);
    bool OnAudioSourceDeleted(const OnDeleted<AudioSource>& event);
}