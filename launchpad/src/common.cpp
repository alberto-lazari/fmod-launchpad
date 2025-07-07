#include "common.h"
#include "fmod_errors.h"

#include <iostream>
#include <format>

void Guard(FMOD_RESULT rc)
{
    if (rc != FMOD_OK)
    {
        std::cerr << std::format("FMOD error {}: {}\n", (int)rc, FMOD_ErrorString(rc));
        exit(rc);
    }
}
