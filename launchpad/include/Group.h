#pragma once

#include <string>

namespace FMOD
{

class System;
class ChannelGroup;

} // namespace FMOD

class Group
{
    // Needs to access raw group pointer to play sounds
    friend class Launchpad;

private:
    std::string name;
    FMOD::System* system;
    FMOD::ChannelGroup* group {};
    float panLevel = 0.f;
    bool ownsGroup = true;

    Group(
        FMOD::System* system,
        const std::string& name,
        FMOD::ChannelGroup* group
    );

public:
    Group(FMOD::System* system, const std::string& name);

    // Prevent copy, define only move
    Group(const Group&) = delete;
    Group& operator=(const Group&) = delete;

    Group(Group&& other) noexcept;
    Group& operator=(Group&& other) noexcept;

    // Release owned resource
    ~Group();


    void toggleMute();
    void togglePlayPause();
    void stop();
    void changeVolume(bool volumeUp, float step = 0.1f);
    void pan(bool panRight, float step = 0.1f);

    void addGroup(const Group& subGroup);
    const std::string& getName() const { return name; }
    std::string getInternalName();

    bool isMuted();
    bool isPaused();
    float getVolume();
    float getPanLevel() const { return panLevel; };

    static Group& Master(FMOD::System* system);
};
