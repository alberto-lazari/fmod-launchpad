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
        makeMode(i_params)
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
        if (channel) channel->stop();
        if (sound) Guard(sound->release());

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
    if (channel) channel->stop();
    if (sound) Guard(sound->release());
}


std::string Sound::getName()
{
    std::string fullName = name;
    if (isLoop()) fullName += " [L]";
    if (isStream()) fullName += " [S]";
    return fullName;
}

bool Sound::isPlaying()
{
    if (!channel) return false;

    bool isPlaying = false;
    if ((channel->isPlaying(&isPlaying)) != FMOD_OK) return false;

    return isPlaying;
}

bool Sound::isLoop()
{
    if (!sound) return false;

    FMOD_MODE mode;
    Guard(sound->getMode(&mode));
    return mode & FMOD_LOOP_NORMAL;
}

bool Sound::isStream()
{
    if (!sound) return false;

    FMOD_MODE mode;
    Guard(sound->getMode(&mode));
    return mode & FMOD_CREATESTREAM;
}

void Sound::play(FMOD::ChannelGroup* i_group)
{
    if (!sound) return;

    if (isPlaying())
    {
        // If sound is already playing stop it and restart. Do not spawn multiple instances
        Guard(channel->stop());
    }

    Guard(system->playSound(sound, i_group, false, &channel));
}


FMOD_MODE Sound::makeMode(const Params& i_params)
{
    FMOD_MODE mode = FMOD_DEFAULT;

    if (i_params.loop) mode |= FMOD_LOOP_NORMAL;
    if (i_params.stream) mode |= FMOD_CREATESTREAM;

    return mode;
}
