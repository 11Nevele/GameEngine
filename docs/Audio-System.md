# Audio System Documentation

The Achoium Engine features a comprehensive audio system built on SoLoud, providing 3D spatial audio, multiple audio formats support, and flexible audio management.

## Overview

The audio system consists of:
- **AudioManager**: Central audio resource management
- **AudioSource**: Component for entities that emit sound
- **AudioListener**: Component for entities that receive audio (typically cameras)
- **SoLoud Integration**: High-performance audio backend

## Audio Components

### AudioSource Component

Represents an entity that can play audio:

```cpp
struct AudioSource {
    AudioID audioID = 0;                  // ID of audio clip to play
    float volume = 1.0f;                  // Volume (0.0 to 1.0)
    float pitch = 1.0f;                   // Pitch multiplier
    bool loop = false;                    // Loop the audio
    bool playOnAwake = false;             // Auto-play when component added
    bool is3D = true;                     // Enable 3D positioning
    float minDistance = 1.0f;             // Distance for maximum volume
    float maxDistance = 100.0f;           // Distance where audio becomes inaudible
    float rolloffFactor = 1.0f;           // How quickly volume decreases with distance
    
    // Runtime state
    bool isPlaying = false;               // Currently playing
    SoLoud::handle handle = 0;            // SoLoud playback handle
};

// Usage
Entity soundEntity = world.CreateEntity("SoundSource");
world.Add<Transform>(soundEntity, Transform{{10, 0, 0}});

AudioSource audioSource;
audioSource.audioID = audioManager.GetAudioID("ExplosionSound");
audioSource.volume = 0.8f;
audioSource.is3D = true;
audioSource.minDistance = 5.0f;
audioSource.maxDistance = 50.0f;
world.Add<AudioSource>(soundEntity, audioSource);
```

### AudioListener Component

Represents an entity that receives audio (like a camera or player):

```cpp
struct AudioListener {
    bool active = true;                   // Is this listener active
};

// Usage - typically on camera
Entity camera = world.CreateEntity("Camera");
world.Add<Transform>(camera, Transform{});
world.Add<AudioListener>(camera, AudioListener{true});
```

## AudioManager

The AudioManager handles loading, storing, and playing audio files:

### Loading Audio

```cpp
AudioManager& audioManager = world.GetResourse<AudioManager>();

// Register audio files
AudioID explosionID = audioManager.RegisterAudio("Explosion", "Assets/Audio/explosion.wav");
AudioID musicID = audioManager.RegisterAudio("BackgroundMusic", "Assets/Audio/background.ogg");
AudioID jumpID = audioManager.RegisterAudio("Jump", "Assets/Audio/jump.wav");

// Get audio ID by name
AudioID soundID = audioManager.GetAudioID("Explosion");
```

### Playing Audio

```cpp
// Play audio directly (2D, non-positional)
audioManager.Play(explosionID, false, 1.0f); // No loop, full volume
audioManager.PlayByName("Jump", false, 0.7f); // By name, 70% volume

// Play with looping
audioManager.Play(musicID, true, 0.5f); // Loop background music at 50% volume
```

### Audio Control

```cpp
// Stop specific audio
audioManager.Stop(musicID);
audioManager.StopByName("BackgroundMusic");

// Stop all audio
audioManager.StopAll();

// Pause and resume
audioManager.Pause(musicID);
audioManager.Resume(musicID);

// Master volume control
audioManager.SetMasterVolume(0.8f); // 80% master volume
float volume = audioManager.GetMasterVolume();

// Mute/unmute
audioManager.SetMuted(true);
bool isMuted = audioManager.IsMuted();
```

## 3D Spatial Audio

### Positioning

The audio system automatically handles 3D positioning using Transform components:

```cpp
void Create3DAudioSource(World& world, glm::vec3 position, const std::string& audioName)
{
    Entity audioEntity = world.CreateEntity("3DAudioSource");
    
    // Position in 3D space
    world.Add<Transform>(audioEntity, Transform{position});
    
    // Audio source with 3D settings
    AudioSource audioSource;
    audioSource.audioID = world.GetResourse<AudioManager>().GetAudioID(audioName);
    audioSource.is3D = true;
    audioSource.minDistance = 2.0f;      // Full volume within 2 units
    audioSource.maxDistance = 20.0f;     // Inaudible beyond 20 units
    audioSource.rolloffFactor = 1.0f;    // Linear falloff
    
    world.Add<AudioSource>(audioEntity, audioSource);
}
```

### Distance-Based Volume

Configure how audio volume changes with distance:

```cpp
AudioSource& source = world.Get<AudioSource>(entity);

// Linear rolloff (default)
source.rolloffFactor = 1.0f;
source.minDistance = 1.0f;   // Full volume within 1 unit
source.maxDistance = 10.0f;  // Silent beyond 10 units

// Slower falloff (larger distances)
source.rolloffFactor = 0.5f;
source.maxDistance = 50.0f;

// Faster falloff (shorter range)
source.rolloffFactor = 2.0f;
source.maxDistance = 5.0f;
```

### Multiple Listeners

Support for multiple audio listeners (useful for split-screen):

```cpp
// Player 1 listener
Entity player1Camera = world.CreateEntity("Player1Camera");
world.Add<Transform>(player1Camera, Transform{{0, 0, 0}});
world.Add<AudioListener>(player1Camera, AudioListener{true});

// Player 2 listener
Entity player2Camera = world.CreateEntity("Player2Camera");
world.Add<Transform>(player2Camera, Transform{{100, 0, 0}});
world.Add<AudioListener>(player2Camera, AudioListener{true});

// The audio system will blend audio from both perspectives
```

## Audio System Implementation

### Built-in Audio Systems

The engine registers these audio systems automatically:

- **AudioSystem::UpdateAudio** (Post-Update, Priority 0): Updates 3D audio positioning and handles playback

### Custom Audio Systems

Create custom audio behavior:

```cpp
void DynamicMusicSystem(World& world)
{
    static float musicTimer = 0.0f;
    static bool combatMusic = false;
    
    Time& time = world.GetResourse<Time>();
    AudioManager& audio = world.GetResourse<AudioManager>();
    
    musicTimer += time.Delta();
    
    // Check if player is in combat
    bool inCombat = CheckPlayerInCombat(world);
    
    if (inCombat && !combatMusic)
    {
        // Switch to combat music
        audio.StopByName("PeacefulMusic");
        audio.PlayByName("CombatMusic", true, 0.7f);
        combatMusic = true;
    }
    else if (!inCombat && combatMusic && musicTimer > 2.0f)
    {
        // Switch back to peaceful music after 2 seconds
        audio.StopByName("CombatMusic");
        audio.PlayByName("PeacefulMusic", true, 0.5f);
        combatMusic = false;
        musicTimer = 0.0f;
    }
}

// Register the system
world.AddUpdateSystem(DynamicMusicSystem, 2);
```

### Audio Trigger System

Create audio triggers for environmental sounds:

```cpp
struct AudioTrigger {
    std::string audioName;
    float triggerRadius = 5.0f;
    bool triggered = false;
    float cooldown = 0.0f;
    float cooldownTime = 2.0f;
};

void AudioTriggerSystem(World& world)
{
    Time& time = world.GetResourse<Time>();
    AudioManager& audio = world.GetResourse<AudioManager>();
    
    // Get player position
    auto players = world.View<Transform, PlayerTag>();
    if (players.begin() == players.end()) return;
    
    Entity player = *players.begin();
    Transform& playerTransform = world.Get<Transform>(player);
    
    // Check audio triggers
    auto triggers = world.View<Transform, AudioTrigger>();
    for (Entity entity : triggers)
    {
        Transform& triggerTransform = world.Get<Transform>(entity);
        AudioTrigger& trigger = world.Get<AudioTrigger>(entity);
        
        if (trigger.cooldown > 0)
        {
            trigger.cooldown -= time.Delta();
            continue;
        }
        
        float distance = glm::length(playerTransform.position - triggerTransform.position);
        
        if (distance <= trigger.triggerRadius && !trigger.triggered)
        {
            // Play audio at trigger location
            audio.PlayByName(trigger.audioName, false, 1.0f);
            trigger.triggered = true;
            trigger.cooldown = trigger.cooldownTime;
        }
        else if (distance > trigger.triggerRadius)
        {
            trigger.triggered = false;
        }
    }
}
```

## Audio Events

### Audio Component Events

The system generates events when audio components are added/removed:

```cpp
bool OnAudioSourceAdded(const OnAdded<AudioSource>& event)
{
    Entity entity = event.entityID;
    AudioSource& source = event.component;
    World& world = event.world;
    
    ACMSG("Audio source added to entity " << entity);
    
    // Auto-play if configured
    if (source.playOnAwake && source.audioID != 0)
    {
        AudioManager& audio = world.GetResourse<AudioManager>();
        if (source.is3D && world.Has<Transform>(entity))
        {
            // Play 3D audio (handled by audio system)
            source.isPlaying = true;
        }
        else
        {
            // Play 2D audio immediately
            audio.Play(source.audioID, source.loop, source.volume);
        }
    }
    
    return true;
}

bool OnAudioSourceRemoved(const OnDeleted<AudioSource>& event)
{
    AudioSource& source = event.component;
    
    // Stop the audio if it's playing
    if (source.isPlaying && source.handle != 0)
    {
        SoLoud::Soloud& soloud = GetSoLoudInstance(); // Get global SoLoud instance
        soloud.stop(source.handle);
    }
    
    return true;
}
```

### Custom Audio Events

Define your own audio-related events:

```cpp
struct AudioFinishedEvent {
    Entity sourceEntity;
    AudioID audioID;
};

struct MusicChangeEvent {
    std::string newTrack;
    float fadeTime;
};

// Event handlers
bool OnAudioFinished(const AudioFinishedEvent& event)
{
    ACMSG("Audio finished playing on entity " << event.sourceEntity);
    
    // Maybe trigger next audio in sequence
    // Or remove the entity if it was a one-shot sound effect
    
    return true;
}

bool OnMusicChange(const MusicChangeEvent& event)
{
    AudioManager& audio = world.GetResourse<AudioManager>();
    
    // Fade out current music and start new track
    // Implementation would depend on your fade system
    
    return true;
}
```

## Audio Optimization

### Audio Pooling

Pool audio sources for frequently played sounds:

```cpp
class AudioPool {
private:
    std::vector<Entity> availableSources;
    World* world;
    
public:
    AudioPool(World& w, size_t poolSize) : world(&w) {
        // Pre-create audio source entities
        for (size_t i = 0; i < poolSize; ++i) {
            Entity source = world->CreateEntity("PooledAudioSource");
            world->Add<Transform>(source, Transform{});
            world->Add<AudioSource>(source, AudioSource{});
            availableSources.push_back(source);
        }
    }
    
    Entity GetAudioSource() {
        if (availableSources.empty()) {
            // Create new source if pool is empty
            Entity source = world->CreateEntity("PooledAudioSource");
            world->Add<Transform>(source, Transform{});
            world->Add<AudioSource>(source, AudioSource{});
            return source;
        }
        
        Entity source = availableSources.back();
        availableSources.pop_back();
        return source;
    }
    
    void ReturnAudioSource(Entity source) {
        // Reset audio source state
        AudioSource& audioSource = world->Get<AudioSource>(source);
        audioSource.isPlaying = false;
        audioSource.handle = 0;
        
        availableSources.push_back(source);
    }
};

// Usage
void PlayPooledSound(AudioPool& pool, const std::string& soundName, glm::vec3 position)
{
    Entity source = pool.GetAudioSource();
    
    Transform& transform = world.Get<Transform>(source);
    transform.position = position;
    
    AudioSource& audio = world.Get<AudioSource>(source);
    audio.audioID = audioManager.GetAudioID(soundName);
    audio.playOnAwake = true;
    
    // Set up a timer to return to pool when finished
    world.Add<AudioPoolTimer>(source, AudioPoolTimer{3.0f, &pool});
}
```

### Culling Distant Audio

Automatically disable audio sources that are too far away:

```cpp
void AudioCullingSystem(World& world)
{
    // Find the primary audio listener
    auto listeners = world.View<Transform, AudioListener>();
    if (listeners.begin() == listeners.end()) return;
    
    Entity primaryListener = *listeners.begin();
    Transform& listenerTransform = world.Get<Transform>(primaryListener);
    
    // Cull distant audio sources
    auto audioSources = world.View<Transform, AudioSource>();
    for (Entity entity : audioSources)
    {
        Transform& sourceTransform = world.Get<Transform>(entity);
        AudioSource& source = world.Get<AudioSource>(entity);
        
        float distance = glm::length(listenerTransform.position - sourceTransform.position);
        
        // Disable sources beyond maximum hearing distance
        if (distance > 100.0f && source.isPlaying)
        {
            // Pause distant audio to save CPU
            SoLoud::Soloud& soloud = GetSoLoudInstance();
            soloud.setPause(source.handle, true);
        }
        else if (distance <= 100.0f && source.isPlaying)
        {
            // Resume audio when back in range
            SoLoud::Soloud& soloud = GetSoLoudInstance();
            soloud.setPause(source.handle, false);
        }
    }
}
```

## Integration Examples

### Footstep System

```cpp
struct Footsteps {
    std::vector<std::string> footstepSounds;
    float stepInterval = 0.5f;
    float stepTimer = 0.0f;
    int currentSound = 0;
};

void FootstepSystem(World& world)
{
    Time& time = world.GetResourse<Time>();
    
    auto walkers = world.View<Transform, RigidBody, Footsteps>();
    for (Entity entity : walkers)
    {
        RigidBody& rb = world.Get<RigidBody>(entity);
        Footsteps& footsteps = world.Get<Footsteps>(entity);
        
        // Check if entity is moving
        float speed = glm::length(rb.velocity);
        if (speed > 1.0f) // Walking threshold
        {
            footsteps.stepTimer += time.Delta();
            
            // Adjust step interval based on speed
            float adjustedInterval = footsteps.stepInterval / (speed / 5.0f);
            
            if (footsteps.stepTimer >= adjustedInterval)
            {
                // Play footstep sound
                if (!footsteps.footstepSounds.empty())
                {
                    const std::string& soundName = footsteps.footstepSounds[footsteps.currentSound];
                    PlaySoundAtPosition(world, soundName, world.Get<Transform>(entity).position);
                    
                    footsteps.currentSound = (footsteps.currentSound + 1) % footsteps.footstepSounds.size();
                }
                
                footsteps.stepTimer = 0.0f;
            }
        }
        else
        {
            footsteps.stepTimer = 0.0f;
        }
    }
}
```

### Interactive Audio Objects

```cpp
struct InteractiveAudio {
    std::string interactSound;
    bool canInteract = true;
    float interactCooldown = 1.0f;
    float cooldownTimer = 0.0f;
};

void InteractiveAudioSystem(World& world)
{
    Time& time = world.GetResourse<Time>();
    InputManager& input = world.GetResourse<InputManager>();
    
    // Update cooldowns
    auto interactives = world.View<InteractiveAudio>();
    for (Entity entity : interactives)
    {
        InteractiveAudio& interactive = world.Get<InteractiveAudio>(entity);
        if (interactive.cooldownTimer > 0)
        {
            interactive.cooldownTimer -= time.Delta();
            if (interactive.cooldownTimer <= 0)
            {
                interactive.canInteract = true;
            }
        }
    }
    
    // Check for interaction
    if (input.IsKeyDown(AC_KEY_E))
    {
        Entity player = GetPlayer(world);
        if (player == NULL_ENTITY) return;
        
        Transform& playerTransform = world.Get<Transform>(player);
        
        // Find nearby interactive objects
        for (Entity entity : interactives)
        {
            Transform& objectTransform = world.Get<Transform>(entity);
            InteractiveAudio& interactive = world.Get<InteractiveAudio>(entity);
            
            float distance = glm::length(playerTransform.position - objectTransform.position);
            
            if (distance <= 3.0f && interactive.canInteract) // Within 3 units
            {
                // Play interaction sound
                PlaySoundAtPosition(world, interactive.interactSound, objectTransform.position);
                
                interactive.canInteract = false;
                interactive.cooldownTimer = interactive.interactCooldown;
                
                break; // Only interact with one object at a time
            }
        }
    }
}
```

## Supported Audio Formats

The SoLoud backend supports many audio formats:
- **WAV** - Uncompressed, best quality
- **OGG** - Compressed, good quality/size ratio
- **MP3** - Compressed, widely supported
- **FLAC** - Lossless compression

Choose formats based on your needs:
- Use **WAV** for short sound effects
- Use **OGG** for music and longer sounds
- Use **MP3** for compatibility if needed
- Use **FLAC** for high-quality music

The Achoium audio system provides a complete solution for game audio needs, from simple sound effects to complex 3D spatial audio environments.