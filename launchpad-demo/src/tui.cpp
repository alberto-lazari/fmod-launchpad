#include "tui.h"

Key::Key()
    : Key(CHAR_NULL)
{
}

Key::Key(EnumType v)
    : value(v)
    , keyChar(charOf(v))
{
}

Key::Key(char c)
    : value(fromChar(c))
    , keyChar(c)
{
}

char Key::charOf(EnumType e)
{
    switch (e)
    {
        case MUTE: return CHAR_MUTE;
        case PLAY_PAUSE: return CHAR_PLAY_PAUSE;
        case QUIT: return CHAR_QUIT;
        case STOP: return CHAR_STOP;
        default: return CHAR_NULL;
    }
}

Key::EnumType Key::fromChar(char c)
{
    switch (c)
    {
        case CHAR_MUTE: return MUTE;
        case CHAR_PLAY_PAUSE: return PLAY_PAUSE;
        case CHAR_QUIT: return QUIT;
        case CHAR_STOP: return STOP;
        case CHAR_NEXT_GROUP: return NEXT_GROUP;
        case CHAR_PREV_GROUP: return PREV_GROUP;
        default: return isSoundKey(c) ? PLAY_SOUND : OTHER;
    }
}

#ifdef _WIN32
    #include <conio.h>
    char getch()
    {
        return _getch();
    }
#else
    #include <termios.h>
    #include <unistd.h>

    char getch()
    {
        termios oldt, newt;
        char c;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        read(STDIN_FILENO, &c, 1);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return c;
    }
#endif

Key getKey()
{
    char c = getch();
    if (c != Key::CHAR_ESC) return Key(c);

    // Detect escape sequence for arrows
    if (getch() != '[') return Key::OTHER;
    switch(getch())
    {
        case Key::CHAR_UP_ARROW: return Key::NEXT_GROUP;
        case Key::CHAR_DOWN_ARROW: return Key::PREV_GROUP;
        default: return Key::OTHER;
    }
}
