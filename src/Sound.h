#pragma once

#include "fmod.hpp"

#include <string>

class Sound
{
public:
    struct Params
    {
        std::string path;
        std::string name;
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


    void play(FMOD::ChannelGroup* group);
};
