#pragma once

#include "fmod.hpp"
#include "Sound.h"

#include <map>
#include <vector>
#include <string>

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
        const std::vector<std::string>& channels
    );
    ~Launchpad();

    void mainLoop();

    void addSound(char key, const Sound::Params& params);
    void addGroup(const std::string& name);

    void muteGroup(FMOD::ChannelGroup* group);
    void stopGroup(FMOD::ChannelGroup* group);
    void playSound(char key, FMOD::ChannelGroup* group);
    void togglePlayPause();

private:
    static FMOD::System* System_Init();

    std::string getGroupName(FMOD::ChannelGroup* group);
};
