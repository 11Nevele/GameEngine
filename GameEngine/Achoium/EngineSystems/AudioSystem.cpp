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
        // ��ȡ��Ƶ������
        AudioManager& audioManager = world.GetResourse<AudioManager>();

        // ���һ�Ծ����Ƶ��������ͨ�����ӵ���һ������
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

        // ����������ƵԴ���
        world.View<AudioSource>().ForEach([&audioManager,&world, activeListenerEntity](Entity entity, AudioSource& audioSource)
            {
                // ������Ч����ƵID
                if (audioSource.audioID == INVALID_AUDIO_ID) return;

                // ��ȡ��Ƶ����
                AudioClip* clip = audioManager.GetAudioClip(audioSource.audioID);
                if (!clip) return;

                // ���ھ��������˥��������л�Ծ�ļ������ͱ任�����
                if (activeListenerEntity != NULL_ENTITY &&
                    world.Has<Transform>(entity) &&
                    world.Has<Transform>(activeListenerEntity))
                {
                    const auto& sourceTransform = world.Get<Transform>(entity);
                    const auto& listenerTransform = world.Get<Transform>(activeListenerEntity);

                    // ����Դ�������֮��ľ���
                    float distance = glm::distance(sourceTransform.position, listenerTransform.position);

                    // ���ݾ�������������򵥵�����˥����
                    const float maxDistance = 20.0f; // ����������
                    if (distance > maxDistance)
                    {
                        // ���������룬ֹͣ����
                        if (audioSource.isPlaying)
                        {
                            audioManager.Stop(audioSource.audioID);
                            auto& source = world.Get<AudioSource>(entity);
                            source.isPlaying = false;
                        }
                    }
                    else
                    {
                        // ���ݾ��������������
                        float volumeScale = 1.0f - (distance / maxDistance);

                        // ��ǰPlaySound API��֧�ֶ�̬��������
                        // ��ʵ����Ŀ�У�Ӧʹ��֧�ִ˹��ܵ���Ƶ�⣨��FMOD��

                        // �����Ƶ��ǰδ���ŵ�Ӧ�ò��ţ������¿�ʼ����
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