#include "Group.h"
#include "common.h"

#include <optional>

Group::Group(
    FMOD::System* i_system,
    const std::string& i_name,
    FMOD::ChannelGroup* i_group
)
    : name(i_name)
    , system(i_system)
    , group(i_group)
    , ownsGroup(false)
{
}

Group::Group(FMOD::System* i_system, const std::string& i_name)
    : name(i_name)
    , system(i_system)
{
    Guard(system->createChannelGroup(i_name.c_str(), &group));
}

Group::Group(Group&& other) noexcept
    : name(other.name)
    , system(other.system)
    , group(other.group)
    , ownsGroup(other.ownsGroup)
{
    // Prevent other object to release moved resources
    if (other.ownsGroup)
    {
        other.group = nullptr;
    }
}

Group& Group::operator=(Group&& other) noexcept
{
    if (this != &other)
    {
        // Release existing group if it already owns one
        if (ownsGroup && group)
        {
            group->stop();
            Guard(group->release());
        }

        name = other.name;
        system = other.system;
        group = other.group;
        ownsGroup = other.ownsGroup;

        if (other.ownsGroup)
        {
            other.group = nullptr;
        }
    }
    return *this;
}

Group::~Group()
{
    if (ownsGroup && group)
    {
        group->stop();
        Guard(group->release());
    }
}

void Group::toggleMute()
{
    if (!group) return;

    bool isMuted;
    Guard(group->getMute(&isMuted));
    Guard(group->setMute(!isMuted));
}

void Group::togglePlayPause()
{
    if (!group) return;

    bool isPaused;
    Guard(group->getPaused(&isPaused));
    Guard(group->setPaused(!isPaused));
}

void Group::stop()
{
    if (!group) return;

    Guard(group->stop());
}

void Group::addGroup(const Group& i_subGroup)
{
    if (!group) return;

    Guard(group->addGroup(i_subGroup.group));
}

void Group::changeVolume(bool i_volumeUp, float i_step)
{
    if (!group) return;

    float volume;
    Guard(group->getVolume(&volume));
    volume += i_volumeUp ? i_step : -i_step;
    if (volume < 0.f) volume = 0.f;

    Guard(group->setVolume(volume));
}

void Group::pan(bool i_panRight, float i_step)
{
    panLevel += i_panRight ? i_step : -i_step;
    if (panLevel > 1.f) panLevel = 1.f;
    if (panLevel < -1.f) panLevel = -1.f;

    if (group)
    {
        Guard(group->setPan(panLevel));
    }
}

std::string Group::getInternalName()
{
    if (!group) return "[null]";

    char name[256];
    Guard(group->getName(name, sizeof(name)));
    return name;
}

Group& Group::Master(FMOD::System* i_system)
{
    static std::optional<Group> MASTER;
    if (!MASTER)
    {
        // Init master group instance
        FMOD::ChannelGroup* masterGroup {};
        Guard(i_system->getMasterChannelGroup(&masterGroup));
        MASTER = Group(i_system, "Master", masterGroup);
    }
    return *MASTER;
}
