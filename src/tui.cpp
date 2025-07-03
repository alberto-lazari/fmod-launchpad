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
