#pragma once

#include "fmod.hpp"

#include <map>
#include <vector>

class Launchpad
{
private:
    FMOD::System* system {};
    std::map<char, FMOD::Sound*> sounds;
    std::vector<FMOD::ChannelGroup*> groups;
    FMOD::ChannelGroup* masterGroup {};
    FMOD::ChannelGroup* selectedGroup {};

public:
    Launchpad();
    Launchpad(
        const std::map<char, FMOD::Sound*>& sounds,
        const std::vector<FMOD::ChannelGroup*>& channels
    );
    ~Launchpad();

    void playSound(char key);
    void addSound(char key, const char* path);

    void mainLoop();

private:
    static FMOD::System* System_Init();
};
