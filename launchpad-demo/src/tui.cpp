#include "tui.h"

#include <iostream>
#include <format>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

namespace TUI
{

Key::Key()
    : Key(CHAR_NULL)
{
}

Key::Key(EnumType v)
    : value(v)
    , keyChar(CharOf(v))
{
}

Key::Key(char c)
    : value(FromChar(c))
    , keyChar(c)
{
}

char Key::CharOf(EnumType e)
{
    switch (e)
    {
        case MUTE: return CHAR_MUTE;
        case PLAY_PAUSE: return CHAR_PLAY_PAUSE;
        case QUIT: return CHAR_QUIT;
        case STOP: return CHAR_STOP;
        case NEXT_GROUP: return CHAR_NEXT_GROUP;
        case PREV_GROUP: return CHAR_PREV_GROUP;
        case VOLUME_UP: return CHAR_VOLUME_UP;
        case VOLUME_DOWN: return CHAR_VOLUME_DOWN;
        case PAN_LEFT: return CHAR_PAN_LEFT;
        case PAN_RIGHT: return CHAR_PAN_RIGHT;
        case HELP: return CHAR_HELP;
        default: return CHAR_NULL;
    }
}

Key::EnumType Key::FromChar(char c)
{
    switch (c)
    {
        case CHAR_MUTE: return MUTE;
        case CHAR_PLAY_PAUSE: return PLAY_PAUSE;
        case CHAR_QUIT: return QUIT;
        case CHAR_STOP: return STOP;
        case CHAR_NEXT_GROUP: return NEXT_GROUP;
        case CHAR_PREV_GROUP: return PREV_GROUP;
        case CHAR_VOLUME_UP: return VOLUME_UP;
        case CHAR_VOLUME_DOWN: return VOLUME_DOWN;
        case CHAR_PAN_LEFT: return PAN_LEFT;
        case CHAR_PAN_RIGHT: return PAN_RIGHT;
        case CHAR_HELP: return HELP;
        default: return IsSoundKey(c) ? PLAY_SOUND : OTHER;
    }
}

char getch()
{
#ifdef _WIN32
    return _getch();
#else
    termios oldt, newt;
    char c;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    read(STDIN_FILENO, &c, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
#endif
}

Key getKey()
{
    char c = getch();

    // Detect escape sequence for arrows
#ifdef _WIN32
    if (c == Key::CHAR_WIN_ESCAPE)
    {
#else
    if (c == Key::CHAR_ESC)
    {
        if (getch() != '[') return Key::OTHER;
#endif
        switch (getch())
        {
            case Key::CHAR_DOWN_ARROW: return Key::NEXT_GROUP;
            case Key::CHAR_UP_ARROW: return Key::PREV_GROUP;
            case Key::CHAR_LEFT_ARROW: return Key::PAN_LEFT;
            case Key::CHAR_RIGHT_ARROW: return Key::PAN_RIGHT;
            default: return Key::OTHER;
        }
    }

    return Key(c);
}


// Clear display, moving current line on top and move cursor to home position
void initScreen()
{
#ifdef _WIN32
    // Enable ANSI codes support on Windows
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
    std::cout << "\033[2J\033[H" << std::flush;
}

// Move cursor to home position and clear all lines below
void clearScreen()
{
    std::cout << "\033[H\033[J" << std::flush;
}

void hideCursor()
{
    std::cout << "\033[?25l" << std::flush;
}

void showCursor()
{
    std::cout << "\033[?25h" << std::flush;
}


void printHelp()
{
    clearScreen();

    std::cout
        << "============= Available Commands =============\n"
        << "  a-z,0-9    Play a sound\n"
        << "  Space      Play/Pause Master channel\n"
        << std::format("  {}/Down     Select next channel\n", Key::CHAR_NEXT_GROUP)
        << std::format("  {}/Up       Select previous channel\n", Key::CHAR_PREV_GROUP)
        << std::format("  {}          Increment current channel volume\n", Key::CHAR_VOLUME_UP)
        << std::format("  {}          Decrement current channel volume\n", Key::CHAR_VOLUME_DOWN)
        << std::format("  {}          Mute current channel\n", Key::CHAR_MUTE)
        << std::format("  {}          Stop current channel\n", Key::CHAR_STOP)
        << std::format("  {}/Left     Pan current channel to left\n", Key::CHAR_PAN_LEFT)
        << std::format("  {}/Right    Pan current channel to right\n", Key::CHAR_PAN_RIGHT)
        << std::format("  {}          Quit\n", Key::CHAR_QUIT)
        << std::format("  {}          Show this message\n", Key::CHAR_HELP)
        << "==============================================\n"
        << "    Press any key to go back to Launchpad"
        << std::flush;

    getch();
}

} // namespace TUI
