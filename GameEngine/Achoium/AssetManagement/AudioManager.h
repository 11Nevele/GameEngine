#pragma once
#include "acpch.h"

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