#include "Launchpad.h"
#include "tui.h"
using TUI::Key;

#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

void printLaunchpadUI(Launchpad& launchpad)
{
    TUI::clearScreen();
    launchpad.dump();
    std::cout
        << "==============================================\n"
        << "               Press ? for help"
        << std::flush;
}

void mainLoop(Launchpad& launchpad)
{
    Key key;
    while ((key = TUI::getKey()) != Key::QUIT)
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
            case Key::HELP:
                TUI::printHelp();
                break;
            default:
                launchpad.playSound(key.keyChar);
                break;
        }

        printLaunchpadUI(launchpad);
    }
}

// Try finding audio directoy in current path or in executable directory
fs::path audioDir(const char* argv0, const std::string& dirName) {
    fs::path invokedPath = argv0;
    fs::path local = dirName;
    if (fs::exists(local) && fs::is_directory(local))
    {
        return local;
    }

    fs::path exeRelative = fs::absolute(invokedPath).parent_path() / dirName;
    if (fs::exists(exeRelative) && fs::is_directory(exeRelative))
    {
        return exeRelative;
    }

    std::cerr << "error: could not find 'audio/' directory.\n"
        << std::format("Tried paths: './{}' and '{}'\n",
            local.string(),
            exeRelative.string()
        );
    exit(1);
}

int main(int, const char* argv[])
{
    std::vector<std::pair<char, Sound::Params>> sounds
    {
        { 'd', { .name = "Drums", .file = "drums-loop.wav", .loop = true } },
        { 'b', { .name = "Bass drums", .file = "bass-drums-loop.wav", .loop = true } },
        { 'v', { .name = "Verse strum", .file = "verse-strum-loop.wav", .loop = true } },
        { 'c', { .name = "Chorus strum", .file = "chorus-strum-loop.wav", .loop = true } },

        { 'i', { .name = "Drums intro", .file = "drums-intro.wav" } },
        { 'g', { .name = "Guitar intro", .file = "guitar-intro.wav" } },
        { '1', { .name = "Fill 1", .file = "fill-1.wav" } },
        { '2', { .name = "Fill 2", .file = "fill-2.wav" } },
        { '3', { .name = "Fill 3", .file = "fill-3.wav" } },
        { 'y', { .name = "Chorus fill syncopated", .file = "chorus-fill-syncopated.wav" } },
        { 's', { .name = "Chorus fill straight", .file = "chorus-fill-straight.wav" } },
        { 'o', { .name = "Guitar solo", .file = "guitar-solo.wav" } },

        { 'f', { .name = "Full mix", .file = "dreams-instrumental.wav", .stream = true } },
    };
    std::vector<std::string> groups
    {
        "Drums",
        "Rythm guitar",
        "Lead guitar",
    };

    TUI::initScreen();
    std::cout << "Initializing system..." << std::endl;
    Launchpad launchpad(audioDir(argv[0], "audio").string(), sounds, groups);

    TUI::hideCursor();
    printLaunchpadUI(launchpad);

    mainLoop(launchpad);

    TUI::clearScreen();
    TUI::showCursor();
    return 0;
}
