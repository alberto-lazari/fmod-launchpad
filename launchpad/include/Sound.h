#pragma once

#include <string>

namespace FMOD
{

class System;
class Sound;
class Channel;
class ChannelGroup;

} // namespace FMOD

typedef unsigned int FMOD_MODE;


class Sound
{
public:
    struct Params
    {
        std::string name;
        std::string path;
        bool loop = false;
        bool stream = false;
    };

private:
    std::string name;
    FMOD::System* system;
    FMOD::Sound* sound {};
    FMOD::Channel* channel {};

public:
    static constexpr const char* AudioDir = "audio/";

    Sound(
        FMOD::System* system,
        const std::string& path,
        const std::string& name,
        FMOD_MODE mode
    );
    Sound(FMOD::System* system, const Params& params);

    // Prevent copy, define only move.
    // Sound must own its pointers
    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;

    Sound(Sound&& other) noexcept;
    Sound& operator=(Sound&& other) noexcept;

    // Release owned resource
    ~Sound();


    std::string getName();
    bool isPlaying();
    bool isLoop();
    bool isStream();

    void play(FMOD::ChannelGroup* group);

private:
    static FMOD_MODE MakeMode(const Params& params);
};
