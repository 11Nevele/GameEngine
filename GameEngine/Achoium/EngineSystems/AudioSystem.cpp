#include "acpch.h"
#include "AudioSystem.h"
#include "../EngineComponents/Audio/Audio.h"
#include "../AssetManagement/AudioManager.h"
#include "../Math/Transform.h"
#include "../Core/World.hpp"

namespace ac
{
    

    void AudioSystem::UpdateAudio(World& world)
    {
        // 获取音频管理器
        AudioManager& audioManager = world.GetResourse<AudioManager>();

        // 查找活跃的音频监听器（通常附加到玩家或相机）
        Entity activeListenerEntity = NULL_ENTITY;
        auto listenerView = world.View<AudioListener>();
        for (auto [entity, listener] : listenerView.GetPacked())
        {
            if (std::get<0>(listener).active)
            {
                activeListenerEntity = entity;
                break;
            }
        }

        // 处理所有音频源组件
        world.View<AudioSource>().ForEach([&audioManager,&world, activeListenerEntity](Entity entity, AudioSource& audioSource)
            {
                // 跳过无效的音频ID
                if (audioSource.audioID == INVALID_AUDIO_ID) return;

                // 获取音频剪辑
                AudioClip* clip = audioManager.GetAudioClip(audioSource.audioID);
                if (!clip) return;

                // 基于距离的音量衰减（如果有活跃的监听器和变换组件）
                if (activeListenerEntity != NULL_ENTITY &&
                    world.Has<Transform>(entity) &&
                    world.Has<Transform>(activeListenerEntity))
                {
                    const auto& sourceTransform = world.Get<Transform>(entity);
                    const auto& listenerTransform = world.Get<Transform>(activeListenerEntity);

                    // 计算源与监听器之间的距离
                    float distance = glm::distance(sourceTransform.position, listenerTransform.position);

                    // 根据距离调整音量（简单的线性衰减）
                    const float maxDistance = 20.0f; // 最大可听距离
                    if (distance > maxDistance)
                    {
                        // 超出最大距离，停止播放
                        if (audioSource.isPlaying)
                        {
                            audioManager.Stop(audioSource.audioID);
                            auto& source = world.Get<AudioSource>(entity);
                            source.isPlaying = false;
                        }
                    }
                    else
                    {
                        // 根据距离计算音量比例
                        float volumeScale = 1.0f - (distance / maxDistance);

                        // 当前PlaySound API不支持动态调整音量
                        // 在实际项目中，应使用支持此功能的音频库（如FMOD）

                        // 如果音频当前未播放但应该播放，则重新开始播放
                        if (!audioSource.isPlaying && audioSource.playOnStart)
                        {
                            float scaledVolume = audioSource.volume * volumeScale;
                            if (audioManager.Play(audioSource.audioID, audioSource.loop, scaledVolume))
                            {
                                auto& source = world.Get<AudioSource>(entity);
                                source.isPlaying = true;
                            }
                        }
                    }
                }
            });
    }
}