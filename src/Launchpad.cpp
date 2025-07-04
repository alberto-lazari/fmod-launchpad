#include "Launchpad.h"

#include "common.h"
#include "tui.h"

#include <iostream>
#include <format>

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
    selectedGroup = masterGroup;

    // Add channel groups to master group hierarchy
    for (const auto& groupName : i_groups)
    {
        addGroup(groupName);
    }
}

Launchpad::~Launchpad()
{
    // Ensure all channels are stopped and system resource is released
    Guard(masterGroup->stop());
    Guard(system->release());
}

void Launchpad::mainLoop()
{
    std::cout << std::format("Group  < {} >\n", getGroupName(selectedGroup));

    char key;
    while ((key = getch()) != KEY_QUIT)
    {
        switch (key)
        {
            case KEY_MUTE:
                bool isMuted;
                Guard(selectedGroup->getMute(&isMuted));
                Guard(selectedGroup->setMute(!isMuted));
                break;
            case KEY_PLAY_PAUSE:
                bool isPaused;
                Guard(masterGroup->getPaused(&isPaused));
                Guard(masterGroup->setPaused(!isPaused));
                break;
            case KEY_STOP:
                Guard(masterGroup->stop());
                break;
            default:
                playSound(key);
                break;
        }
    }
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

void Launchpad::playSound(char i_key)
{
    if (sounds.contains(i_key))
    {
        sounds.at(i_key)
            .play(selectedGroup);
    }
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
    char name[1024];
    Guard(i_group->getName(name, sizeof(name)));
    return i_group == masterGroup ? "Master" : name;
}
