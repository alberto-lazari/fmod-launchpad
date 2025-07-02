#include "common.h"
#include "init.h"
#include "tui.h"

#include <iostream>

int main()
{
    FMOD::System* sys = System_Init();
    FMOD::Channel* channel {};
    FMOD::Sound* sound {};

    Guard(sys->createSound("audio/verse-strum-loop.wav", FMOD_2D, 0, &sound));
    Guard(sys->playSound(sound, 0, true, &channel));
    Guard(channel->setPaused(false));

    std::cout << "Press any button to stop " << std::flush;
    getch();
    std::cout << std::endl;

    Guard(sound->release());
    Guard(sys->release());
    return 0;
}
