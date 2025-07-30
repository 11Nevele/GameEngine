#pragma once
#include "acpch.h"
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
#include <soloud_eqfilter.h>
#include <soloud_error.h>
#include <soloud_fader.h>
#include <soloud_fft.h>
#include <soloud_fftfilter.h>
#include <soloud_file.h>
//#include <soloud_file_hack_off.h>
//#include <soloud_file_hack_on.h>
#include <soloud_filter.h>
#include <soloud_flangerfilter.h>
#include <soloud_freeverbfilter.h>
#include <soloud_internal.h>
#include <soloud_lofifilter.h>
#include <soloud_misc.h>
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
#include <zx7decompress.h>
namespace ac
{
    // 音频剪辑ID类型
    using AudioID = uint32_t;
    const AudioID INVALID_AUDIO_ID = 0; // 无效的音频ID

    // 音频资源类
    struct AudioClip
    {
        AudioID id = INVALID_AUDIO_ID; // 音频唯一ID
        std::string name;              // 音频名称
        std::string filePath;          // 文件路径
        void* data = nullptr;          // 音频数据
        size_t size = 0;               // 数据大小
        bool loaded = false;           // 是否已加载

        AudioClip() = default;
        AudioClip(AudioID id, const std::string& name, const std::string& path) 
            : id(id), name(name), filePath(path) {}
        ~AudioClip() 
        {
            Unload();
        }

        void Load();
        void Unload();
    };

    // 音频管理器 - 作为资源添加到World中
    class AudioManager
    {
    private:
        std::unordered_map<std::string, AudioID> nameToID;         // 名称到ID的映射
        std::unordered_map<AudioID, AudioClip> audioClips;         // ID到音频剪辑的映射
        AudioID nextID = 1;                                        // 下一个可用的音频ID
        float masterVolume = 1.0f;
        bool muted = false;

    public:
        AudioManager();
        ~AudioManager();

        // 注册音频资源并返回分配的ID
        AudioID RegisterAudio(const std::string& name, const std::string& filePath);

        // 通过名称获取音频ID
        AudioID GetAudioID(const std::string& name) const;

        // 通过ID获取音频剪辑
        AudioClip* GetAudioClip(AudioID id);

        // 通过名称获取音频剪辑
        AudioClip* GetAudioClipByName(const std::string& name);

        // 播放音频（使用ID）
        bool Play(AudioID id, bool loop = false, float volume = 1.0f);

        // 播放音频（使用名称 - 为了向后兼容）
        bool PlayByName(const std::string& name, bool loop = false, float volume = 1.0f);

        // 停止音频（使用ID）
        void Stop(AudioID id);

        // 停止音频（使用名称 - 为了向后兼容）
        void StopByName(const std::string& name);

        // 暂停音频
        void Pause(AudioID id);

        // 恢复音频
        void Resume(AudioID id);

        // 停止所有音频
        void StopAll();

        // 设置主音量
        void SetMasterVolume(float volume);

        // 获取主音量
        float GetMasterVolume() const { return masterVolume; }

        // 静音/取消静音
        void SetMuted(bool isMuted);

        // 是否已静音
        bool IsMuted() const { return muted; }
    };
}