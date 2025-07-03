#include "Launchpad.h"

#include "common.h"
#include "tui.h"

Launchpad::Launchpad() : Launchpad({}, {})
{
}

Launchpad::Launchpad(
    const std::map<char, FMOD::Sound*>& i_sounds,
    const std::vector<FMOD::ChannelGroup*>& i_groups
)
    : system(System_Init())
    , sounds(i_sounds)
    , groups(i_groups)
{
    Guard(system->getMasterChannelGroup(&masterGroup));
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

void Launchpad::addSound(char i_key, const char* i_path)
{
    FMOD::Sound* sound;
    Guard(system->createSound(i_path, FMOD_DEFAULT, 0, &sound));
    sounds.insert({ i_key, sound });
}

void Launchpad::playSound(char i_key)
{
    if (sounds.contains(i_key))
    {
        FMOD::Sound* sound = sounds[i_key];
        FMOD::Channel* channel {};
        Guard(system->playSound(sound, selectedGroup, false, &channel));
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
