#include "Launchpad.h"

int main()
{
    Launchpad launchpad;
    launchpad.addSound('v', "audio/verse-strum-loop.wav");

    launchpad.mainLoop();

    return 0;
}
