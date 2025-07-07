#include "Launchpad.h"
#include "common.h"

#include <iostream>
#include <format>

Launchpad::Launchpad() : Launchpad("", {}, {})
{
}

Launchpad::Launchpad(
    const std::string& i_audioDir,
    const std::vector<std::pair<char, Sound::Params>>& i_sounds,
    const std::vector<std::string>& i_groupNames
)
    : system(SystemInit())
{
    // Create sounds
    for (const auto& [key, soundParams] : i_sounds)
    {
        addSound(i_audioDir, key, soundParams);
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

void Launchpad::addSound(const std::string& i_audioDir,
        char i_key, const Sound::Params& i_params)
{
    sounds.insert({ i_key, Sound(system, i_audioDir, i_params) });
    soundKeys.push_back(i_key);
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

void Launchpad::dump()
{
    std::cout << "================== Channels ==================\n";

    for (Group& group : groups)
    {
        float panLevel = group.getPanLevel();
        bool showPan = panLevel < -0.05f || panLevel > 0.05f;
        std::cout << std::format("{} {:<12s} {} {} vol: {:<3.0f}  {}\n",
            &group == &getCurrentGroup() ? ">" : " ",
            group.getName(),
            group.isMuted() ? "[M]" : "   ",
            group.isPaused() ? "|| " : "   ",
            group.getVolume() * 100.f,
            showPan ? std::format("pan: {:1.0f} {}",
                std::abs(panLevel * 10.f),
                panLevel > 0.f ? "right" : "left"
            ) : ""
        );
    }

    std::cout << "=================== Sounds ===================\n";

    for (char key : soundKeys)
    {
        Sound& sound = sounds.at(key);

        std::cout << std::format("{} {}  {}\n",
            sound.isPlaying() ? "~" : " ",
            key,
            sound.getName());
    }

    std::cout << std::flush;
}


FMOD::System* Launchpad::SystemInit()
{
    FMOD::System* sys {};
    Guard(FMOD::System_Create(&sys));
    Guard(sys->init(512, FMOD_INIT_NORMAL, 0));

    return sys;
}
