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

    void addGroup(const Group& subGroup);
    const std::string& getName() const { return name; }
    std::string getInternalName();

    static Group& Master(FMOD::System* system);
};
