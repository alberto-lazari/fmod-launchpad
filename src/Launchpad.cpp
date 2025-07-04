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
    const std::vector<FMOD::ChannelGroup*>& i_groups
)
    : groups(std::vector<FMOD::ChannelGroup*>(i_groups.size() + 1))
    , system(System_Init())
{
    // Create sounds
    for (const auto& [key, soundParams] : i_sounds)
    {
        sounds.insert({ key, Sound(system, soundParams) });
    }

    Guard(system->getMasterChannelGroup(&masterGroup));
    groups.push_back(masterGroup);
    groups.insert(groups.begin(), i_groups.begin(), i_groups.end());

    // Add channel groups to master group hierarchy
    for (FMOD::ChannelGroup* group : i_groups)
    {
        Guard(masterGroup->addGroup(group));
    }

    selectedGroup = masterGroup;
}

Launchpad::~Launchpad()
{
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
        }
    }
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
