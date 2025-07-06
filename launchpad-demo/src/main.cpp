#include "Launchpad.h"
#include "tui.h"

#include <iostream>

int main()
{
    std::map<char, Sound::Params> sounds
    {
        { 'd', { .name = "Drums", .path = "drums-loop.wav", .loop = true } },
        { 'b', { .name = "Bass drums", .path = "bass-drums-loop.wav", .loop = true } },
        { 'v', { .name = "Verse strum", .path = "verse-strum-loop.wav", .loop = true } },
        { 'c', { .name = "Chorus strum", .path = "chorus-strum-loop.wav", .loop = true } },

        { 'i', { .name = "Drums intro", .path = "drums-intro.wav" } },
        { 'g', { .name = "Guitar intro", .path = "guitar-intro.wav" } },
        { '1', { .name = "Fill 1", .path = "fill-1.wav" } },
        { '2', { .name = "Fill 2", .path = "fill-2.wav" } },
        { '3', { .name = "Fill 3", .path = "fill-3.wav" } },
        { 'y', { .name = "Chorus fill syncopated", .path = "chorus-fill-syncopated.wav" } },
        { 's', { .name = "Chorus fill straight", .path = "chorus-fill-straight.wav" } },
        { 'o', { .name = "Guitar solo", .path = "guitar-solo.wav" } },

        { 'f', { .name = "Full mix", .path = "dreams-instrumental.wav", .stream = true } },
    };
    std::vector<std::string> groups
    {
        "Drums",
        "Rythm guitar",
        "Lead guitar",
    };
    std::cout << "//// Launchpad startup... ////" << std::endl;
    Launchpad launchpad(sounds, groups);

    Key key;
    while ((key = getKey()) != Key::QUIT)
    {
        switch (key)
        {
            case Key::MUTE:
                launchpad.groupToggleMute();
                break;
            case Key::PLAY_PAUSE:
                launchpad.togglePlayPause();
                break;
            case Key::STOP:
                launchpad.groupStop();
                break;
            case Key::NEXT_GROUP:
                launchpad.nextGroup();
                break;
            case Key::PREV_GROUP:
                launchpad.previousGroup();
                break;
            case Key::VOLUME_UP:
                launchpad.groupVolumeUp();
                break;
            case Key::VOLUME_DOWN:
                launchpad.groupVolumeDown();
                break;
            case Key::PAN_LEFT:
                launchpad.groupPanLeft();
                break;
            case Key::PAN_RIGHT:
                launchpad.groupPanRight();
                break;
            default:
                launchpad.playSound(key.keyChar);
                break;
        }
    }

    return 0;
}
