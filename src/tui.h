#pragma once

char getch();

struct Key
{
    enum EnumType
    {
        Mute,
        PlayPause,
        Quit,
        Stop,
        PlaySound,
        Other,
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
