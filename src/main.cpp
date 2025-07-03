#include "Launchpad.h"
#include "Sound.h"

int main()
{
    std::map<char, Sound::Params> sounds
    {
        { 'v', { .name = "Verse strum [loop]", .path = "verse-strum-loop.wav", .loop = true } },
    };
    Launchpad launchpad(sounds, {});

    launchpad.mainLoop();

    return 0;
}
