#include "Launchpad.h"
#include "common.h"

Launchpad::Launchpad() : Launchpad({}, {})
{
}

Launchpad::Launchpad(
    const std::map<char, Sound::Params>& i_sounds,
    const std::vector<std::string>& i_groups
)
    : groups(std::vector<FMOD::ChannelGroup*>(i_groups.size() + 1))
    , system(System_Init())
{
    // Create sounds
    for (const auto& [key, soundParams] : i_sounds)
    {
        addSound(key, soundParams);
    }

    Guard(system->getMasterChannelGroup(&masterGroup));
    groups.push_back(masterGroup);

    // Add channel groups to master group hierarchy
    for (const auto& groupName : i_groups)
    {
        addGroup(groupName);
    }
}

Launchpad::~Launchpad()
{
    // Destroy all sounds before closing system
    sounds.clear();
    Guard(system->release());
}

void Launchpad::addSound(char i_key, const Sound::Params& i_params)
{
    sounds.insert({ i_key, Sound(system, i_params) });
}

void Launchpad::addGroup(const std::string& i_name)
{
    FMOD::ChannelGroup* group {};
    Guard(system->createChannelGroup(i_name.c_str(), &group));
    Guard(masterGroup->addGroup(group));
    groups.push_back(group);
}

void Launchpad::muteGroup(FMOD::ChannelGroup* i_group)
{
    if (!i_group) return;

    bool isMuted;
    Guard(i_group->getMute(&isMuted));
    Guard(i_group->setMute(!isMuted));
}

void Launchpad::stopGroup(FMOD::ChannelGroup* i_group)
{
    if (!i_group) return;

    Guard(i_group->stop());
}

void Launchpad::playSound(char i_key, FMOD::ChannelGroup* i_group)
{
    if (!sounds.contains(i_key)) return;

    sounds.at(i_key).play(i_group);
}

void Launchpad::togglePlayPause()
{
    bool isPaused;
    Guard(masterGroup->getPaused(&isPaused));
    Guard(masterGroup->setPaused(!isPaused));
}

FMOD::ChannelGroup* Launchpad::getGroup(unsigned int index) const
{
    if (index >= groups.size()) return nullptr;

    return groups[index];
}


FMOD::System* Launchpad::System_Init()
{
    FMOD::System* sys {};
    Guard(FMOD::System_Create(&sys));
    Guard(sys->init(512, FMOD_INIT_NORMAL, 0));

    return sys;
}

std::string Launchpad::getGroupName(FMOD::ChannelGroup* i_group)
{
    if (!i_group) return "[null]";

    char name[256];
    Guard(i_group->getName(name, sizeof(name)));
    return i_group == masterGroup ? "Master" : name;
}
