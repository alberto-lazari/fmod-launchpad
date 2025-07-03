#include "Launchpad.h"

#include "common.h"
#include "tui.h"

Launchpad::Launchpad() : Launchpad({}, {})
{
}

Launchpad::Launchpad(
    const std::map<char, Sound::Params>& i_sounds,
    const std::vector<FMOD::ChannelGroup*>& i_groups
)
    : groups(i_groups)
    , system(System_Init())
{
    // Create sounds
    for (const auto& [key, soundParams] : i_sounds)
    {
        sounds.insert({ key, Sound(system, soundParams) });
    }

    Guard(system->getMasterChannelGroup(&masterGroup));

    // Add channel groups to master group hierarchy
    for (FMOD::ChannelGroup* group : groups)
    {
        Guard(masterGroup->addGroup(group));
    }
    selectedGroup = groups.size() == 0
        ? masterGroup
        : groups[0];
}

Launchpad::~Launchpad()
{
    Guard(system->release());
}

void Launchpad::playSound(char i_key)
{
    if (sounds.contains(i_key))
    {
        sounds.at(i_key)
            .play(selectedGroup);
    }
}

void Launchpad::mainLoop()
{
    char key;
    while ((key = getch()) != 'Q')
    {
        switch (key)
        {
            case 'M':
                bool isMuted;
                Guard(selectedGroup->getMute(&isMuted));
                Guard(selectedGroup->setMute(!isMuted));
                break;
            case ' ':
                bool isPaused;
                Guard(masterGroup->getPaused(&isPaused));
                Guard(masterGroup->setPaused(!isPaused));
                break;
            default:
                playSound(key);
        }
    }
}


FMOD::System* Launchpad::System_Init()
{
    FMOD::System* sys {};
    Guard(FMOD::System_Create(&sys));
    Guard(sys->init(512, FMOD_INIT_NORMAL, 0));

    return sys;
}
