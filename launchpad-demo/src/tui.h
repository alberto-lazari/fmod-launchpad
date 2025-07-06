#pragma once

struct Key
{
    enum EnumType
    {
        MUTE,
        PLAY_PAUSE,
        QUIT,
        STOP,
        PLAY_SOUND,
        NEXT_GROUP,
        PREV_GROUP,
        OTHER,
    };

    static constexpr char CHAR_NULL = '\0';
    static constexpr char CHAR_MUTE = 'M';
    static constexpr char CHAR_PLAY_PAUSE = ' ';
    static constexpr char CHAR_QUIT = 'Q';
    static constexpr char CHAR_STOP = 'S';
    static constexpr char CHAR_NEXT_GROUP = 'N';
    static constexpr char CHAR_PREV_GROUP = 'P';
    static constexpr char CHAR_ESC = '\x1b';
    static constexpr char CHAR_UP_ARROW = 'A';
    static constexpr char CHAR_DOWN_ARROW = 'B';

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
