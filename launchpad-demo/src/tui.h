#pragma once

namespace TUI
{

struct Key
{
    enum EnumType
    {
        MUTE,
        PLAY_PAUSE,
        QUIT,
        STOP,
        NEXT_GROUP,
        PREV_GROUP,
        VOLUME_UP,
        VOLUME_DOWN,
        PAN_LEFT,
        PAN_RIGHT,
        HELP,
        PLAY_SOUND,
        OTHER,
    };

    // Command keys
    static constexpr char CHAR_NULL         = '\0';
    static constexpr char CHAR_MUTE         = 'M';
    static constexpr char CHAR_PLAY_PAUSE   = ' ';
    static constexpr char CHAR_QUIT         = 'Q';
    static constexpr char CHAR_STOP         = 'S';
    static constexpr char CHAR_NEXT_GROUP   = 'N';
    static constexpr char CHAR_PREV_GROUP   = 'P';
    static constexpr char CHAR_VOLUME_UP    = '+';
    static constexpr char CHAR_VOLUME_DOWN  = '-';
    static constexpr char CHAR_PAN_LEFT     = 'L';
    static constexpr char CHAR_PAN_RIGHT    = 'R';
    static constexpr char CHAR_HELP         = '?';

    // Escape sequence characters
    static constexpr char CHAR_ESC          = '\033';
#ifdef _WIN32
    static constexpr char CHAR_WIN_ESCAPE   = -32;
    static constexpr char CHAR_UP_ARROW     = 72;
    static constexpr char CHAR_DOWN_ARROW   = 80;
    static constexpr char CHAR_RIGHT_ARROW  = 75;
    static constexpr char CHAR_LEFT_ARROW   = 77;
#else
    static constexpr char CHAR_UP_ARROW     = 'A';
    static constexpr char CHAR_DOWN_ARROW   = 'B';
    static constexpr char CHAR_RIGHT_ARROW  = 'C';
    static constexpr char CHAR_LEFT_ARROW   = 'D';
#endif

    Key();
    Key(EnumType value);
    Key(char keyChar);

    operator EnumType() const { return value; }

    static char CharOf(EnumType e);

private:
    EnumType value;

    static EnumType FromChar(char c);

    static bool IsSoundKey(char c)
    {
        // Sound keys are lowercase letters and numbers
        return (c >= 'a' && c <= 'z')
            || (c >= '0' && c <= '9');
    }

public:
    char keyChar;
};

char getch();
Key getKey();

void initScreen();
void clearScreen();
void hideCursor();
void showCursor();

void printHelp();

} // namespace TUI
