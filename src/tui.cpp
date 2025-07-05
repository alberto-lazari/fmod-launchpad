#include "tui.h"

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


Key::Key(char c)
    : value(fromChar(c))
    , keyChar(c)
{
}

Key::EnumType Key::fromChar(char c)
{
    switch (c)
    {
        case 'M': return Mute;
        case ' ': return PlayPause;
        case 'Q': return Quit;
        case 'S': return Stop;
        default:  return isSoundKey(c) ? PlaySound : Other;
    }
}
