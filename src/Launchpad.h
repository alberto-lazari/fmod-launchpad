#pragma once

#include "fmod.hpp"
#include "Sound.h"

#include <map>
#include <vector>

class Launchpad
{
private:
    std::map<char, Sound> sounds;
    std::vector<FMOD::ChannelGroup*> groups;
    FMOD::System* system {};
    FMOD::ChannelGroup* masterGroup {};
    FMOD::ChannelGroup* selectedGroup {};

public:
    Launchpad();
    Launchpad(
        const std::map<char, Sound::Params>& sounds,
        const std::vector<FMOD::ChannelGroup*>& channels
    );
    ~Launchpad();

    void playSound(char key);
    void addSound(char key, const char* path);

    void mainLoop();

private:
    static FMOD::System* System_Init();
};
