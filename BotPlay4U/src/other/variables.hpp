#pragma once
#include <vector>

struct Input {
    size_t ticks[2];
    size_t repeatTimes;
    bool p1;
};

namespace Autoclicker {
    // Input-sequence data retained from the original timing engine.
    extern std::vector<Input> g_inputs[2];
    extern bool g_autoclickerEnabled;

    // BotPlay4U controls. Safe Mode defaults to ON.
    extern bool g_safeModeEnabled;
    extern bool g_hitboxesEnabled;
    extern bool g_safeModeBlocked;
}
