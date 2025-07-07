#pragma once

#include "Sound.h"
#include "Group.h"

#include <map>
#include <vector>
#include <string>
#include <optional>

class Launchpad
{
private:
    std::map<char, Sound> sounds;
    // Keep original sound insertion order
    std::vector<char> soundKeys;
    std::vector<Group> groups;
    FMOD::System* system {};
    size_t currentGroupIndex = 0;

public:
    Launchpad();
    Launchpad(
        const std::string& audioDir,
        const std::vector<std::pair<char, Sound::Params>>& sounds,
        const std::vector<std::string>& groupNames
    );
    ~Launchpad();

    void addSound(const std::string& audioDir, char key, const Sound::Params& params);
    void addGroup(const std::string& name);

    Group* getGroup(const std::string& name);

    Group& nextGroup();
    Group& previousGroup();

    void playSound(char key, const Group& group)
    {
        if (sounds.contains(key))
        {
            sounds.at(key).play(group.group);
        }
    }

    Group& getCurrentGroup() { return groups.at(currentGroupIndex); }

    void playSound(char key)
    {
        if (sounds.contains(key))
        {
            sounds.at(key).play(getCurrentGroup().group);
        }
    }
    void togglePlayPause() { Group::Master(system).togglePlayPause(); }
    void groupToggleMute() { getCurrentGroup().toggleMute(); }
    void groupStop()       { getCurrentGroup().stop(); }
    void groupVolumeUp()   { getCurrentGroup().changeVolume(true); }
    void groupVolumeDown() { getCurrentGroup().changeVolume(false); }
    void groupPanLeft()    { getCurrentGroup().pan(false); }
    void groupPanRight()   { getCurrentGroup().pan(true); }

    void dump();

private:
    static FMOD::System* SystemInit();
};
