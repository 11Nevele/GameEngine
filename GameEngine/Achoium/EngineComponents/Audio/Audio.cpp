#include "acpch.h"
#include "Audio.h"
#include "Debug.h"
#include "Core/World.hpp"
#include "../../AssetManagement/AudioManager.h"

namespace ac
{
    bool OnAudioSourceAdded(const OnAdded<AudioSource>& event)
    {
        // �������Ϊ����ʱ���ţ����Զ���������
        if (event.component.playOnStart && event.component.audioID != INVALID_AUDIO_ID)
        {
            AudioManager& audioManager = event.world.GetResourse<AudioManager>();
            audioManager.Play(event.component.audioID, event.component.loop, event.component.volume);
            // ���²���״̬
            auto& audioSource = event.world.Get<AudioSource>(event.ID);
            audioSource.isPlaying = true;
        }
        return true;
    }

    bool OnAudioSourceDeleted(const OnDeleted<AudioSource>& event)
    {
        // ���ʵ�屻���٣�ֹͣ�����������
        AudioManager& audioManager = event.world.GetResourse<AudioManager>();
        if (event.component.isPlaying && event.component.audioID != INVALID_AUDIO_ID)
        {
            audioManager.Stop(event.component.audioID);
        }
        return true;
    }
}