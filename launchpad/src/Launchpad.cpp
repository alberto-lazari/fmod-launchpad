#include "Launchpad.h"
#include "common.h"

Launchpad::Launchpad() : Launchpad({}, {})
{
}

Launchpad::Launchpad(
    const std::map<char, Sound::Params>& i_sounds,
    const std::vector<std::string>& i_groupNames
)
    : system(SystemInit())
{
    // Create sounds
    for (const auto& [key, soundParams] : i_sounds)
    {
        addSound(key, soundParams);
    }

    groups.push_back(std::move(Group::Master(system)));

    // Add channel groups to master group hierarchy
    for (const auto& groupName : i_groupNames)
    {
        addGroup(groupName);
    }
}

Launchpad::~Launchpad()
{
    // Destroy all sounds and groups before closing system
    sounds.clear();
    groups.clear();

    Guard(system->release());
}

void Launchpad::addSound(char i_key, const Sound::Params& i_params)
{
    sounds.insert({ i_key, Sound(system, i_params) });
}

void Launchpad::addGroup(const std::string& i_name)
{
    Group group(system, i_name);
    Group::Master(system).addGroup(group);
    groups.push_back(std::move(group));
}

Group* Launchpad::getGroup(const std::string& i_name)
{
    for (auto& group : groups)
    {
        if (group.getName() == i_name)
        {
            return &group;
        }
    }
    return nullptr;
}

Group& Launchpad::nextGroup()
{
    currentGroupIndex = (currentGroupIndex + 1) % groups.size();
    return getCurrentGroup();
}

Group& Launchpad::previousGroup()
{
    currentGroupIndex = (currentGroupIndex + groups.size() - 1) % groups.size();
    return getCurrentGroup();
}


FMOD::System* Launchpad::SystemInit()
{
    FMOD::System* sys {};
    Guard(FMOD::System_Create(&sys));
    Guard(sys->init(512, FMOD_INIT_NORMAL, 0));

    return sys;
}
