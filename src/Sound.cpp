#include "Sound.h"
#include "common.h"

Sound::Sound(
    FMOD::System* i_system,
    const std::string& i_path,
    const std::string& i_name,
    FMOD_MODE i_mode
)
    : name(i_name)
    , system(i_system)
{
    FMOD_RESULT rc;
    // Search file in default audio assets directory
    rc = system->createSound((AudioDir + i_path).c_str(), i_mode, nullptr, &sound);

    if (rc == FMOD_ERR_FILE_NOTFOUND)
    {
        // Search file using absolute path
        Guard(system->createSound(i_path.c_str(), i_mode, nullptr, &sound));
    }
}

Sound::Sound(FMOD::System* i_system, const Params& i_params)
    : Sound(
        i_system,
        i_params.path,
        i_params.name,
        FMOD_DEFAULT
        | (i_params.loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT)
        | (i_params.stream ? FMOD_CREATESTREAM : FMOD_DEFAULT)
    )
{
}

Sound::Sound(Sound&& other) noexcept
    : name(std::move(other.name))
    , system(other.system)
    , sound(other.sound)
    , channel(other.channel)
{
    // Prevent other object to release moved resources
    other.sound = nullptr;
    other.channel = nullptr;
}

Sound& Sound::operator=(Sound&& other) noexcept
{
    if (this != &other)
    {
        // Release existing sound if it already owns one
        if (sound)
        {
            sound->release();
        }

        name = std::move(other.name);
        system = other.system;
        sound = other.sound;
        channel = other.channel;

        other.sound = nullptr;
        other.channel = nullptr;
    }
    return *this;
}

Sound::~Sound()
{
    if (sound != nullptr)
    {
        sound->release();
    }
}


void Sound::play(FMOD::ChannelGroup* i_group)
{
    // If sound is already playing stop it and restart. Do not spawn multiple instances
    bool isPlaying = false;
    if (channel != nullptr && Guard(channel->isPlaying(&isPlaying)) && isPlaying)
    {
        channel->stop();
    }

    Guard(system->playSound(sound, i_group, false, &channel));
}
