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
    // ��Ƶ����ID����
    using AudioID = uint32_t;
    const AudioID INVALID_AUDIO_ID = 0; // ��Ч����ƵID

    // ��Ƶ��Դ��
    struct AudioClip
    {
        AudioID id = INVALID_AUDIO_ID; // ��ƵΨһID
        std::string name;              // ��Ƶ����
        std::string filePath;          // �ļ�·��
        void* data = nullptr;          // ��Ƶ����
        size_t size = 0;               // ���ݴ�С
        bool loaded = false;           // �Ƿ��Ѽ���

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

    // ��Ƶ������ - ��Ϊ��Դ��ӵ�World��
    class AudioManager
    {
    private:
        std::unordered_map<std::string, AudioID> nameToID;         // ���Ƶ�ID��ӳ��
        std::unordered_map<AudioID, AudioClip> audioClips;         // ID����Ƶ������ӳ��
        AudioID nextID = 1;                                        // ��һ�����õ���ƵID
        float masterVolume = 1.0f;
        bool muted = false;

    public:
        AudioManager();
        ~AudioManager();

        // ע����Ƶ��Դ�����ط����ID
        AudioID RegisterAudio(const std::string& name, const std::string& filePath);

        // ͨ�����ƻ�ȡ��ƵID
        AudioID GetAudioID(const std::string& name) const;

        // ͨ��ID��ȡ��Ƶ����
        AudioClip* GetAudioClip(AudioID id);

        // ͨ�����ƻ�ȡ��Ƶ����
        AudioClip* GetAudioClipByName(const std::string& name);

        // ������Ƶ��ʹ��ID��
        bool Play(AudioID id, bool loop = false, float volume = 1.0f);

        // ������Ƶ��ʹ������ - Ϊ�������ݣ�
        bool PlayByName(const std::string& name, bool loop = false, float volume = 1.0f);

        // ֹͣ��Ƶ��ʹ��ID��
        void Stop(AudioID id);

        // ֹͣ��Ƶ��ʹ������ - Ϊ�������ݣ�
        void StopByName(const std::string& name);

        // ��ͣ��Ƶ
        void Pause(AudioID id);

        // �ָ���Ƶ
        void Resume(AudioID id);

        // ֹͣ������Ƶ
        void StopAll();

        // ����������
        void SetMasterVolume(float volume);

        // ��ȡ������
        float GetMasterVolume() const { return masterVolume; }

        // ����/ȡ������
        void SetMuted(bool isMuted);

        // �Ƿ��Ѿ���
        bool IsMuted() const { return muted; }
    };
}