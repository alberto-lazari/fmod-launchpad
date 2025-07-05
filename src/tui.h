#pragma once

char getch();

struct Key
{
    enum EnumType
    {
        MUTE,
        PLAY_PAUSE,
        QUIT,
        STOP,
        PLAY_SOUND,
        OTHER,
    };

    Key(char keyChar);

    operator EnumType() const { return value; }

private:
    EnumType value;

    static EnumType fromChar(char c);

    static bool isSoundKey(char c)
    {
        // Sound keys are lowercase letters and numbers
        return (c >= 'a' && c <= 'z')
            || (c >= '0' && c <= '9');
    }

public:
    char keyChar;
};
