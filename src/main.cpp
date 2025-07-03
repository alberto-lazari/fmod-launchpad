#include "Launchpad.h"
#include "Sound.h"

int main()
{
    Launchpad launchpad(
        {
            { 'v', { .path = "verse-strum-loop.wav", .name = "Verse strum [loop]", .loop = true } },
        },
        {}
    );

    launchpad.mainLoop();

    return 0;
}
