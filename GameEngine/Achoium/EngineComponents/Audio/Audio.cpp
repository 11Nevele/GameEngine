#include "acpch.h"
#include "Audio.h"
#include "Debug.h"
#include "Core/World.hpp"
#include "../../AssetManagement/AudioManager.h"

namespace ac
{
    bool OnAudioSourceAdded(const OnAdded<AudioSource>& event)
    {
        // 如果设置为创建时播放，则自动播放声音
        if (event.component.playOnStart && event.component.audioID != INVALID_AUDIO_ID)
        {
            AudioManager& audioManager = event.world.GetResourse<AudioManager>();
            audioManager.Play(event.component.audioID, event.component.loop, event.component.volume);
            // 更新播放状态
            auto& audioSource = event.world.Get<AudioSource>(event.ID);
            audioSource.isPlaying = true;
        }
        return true;
    }

    bool OnAudioSourceDeleted(const OnDeleted<AudioSource>& event)
    {
        // 如果实体被销毁，停止相关声音播放
        AudioManager& audioManager = event.world.GetResourse<AudioManager>();
        if (event.component.isPlaying && event.component.audioID != INVALID_AUDIO_ID)
        {
            audioManager.Stop(event.component.audioID);
        }
        return true;
    }
}