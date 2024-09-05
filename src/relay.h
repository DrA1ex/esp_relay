#pragma once

#include <cstdint>

#include "lib/debug.h"
#include "lib/misc/timer.h"

#include "constants.h"

class RelayManager {
    uint8_t _pin;
    Timer &_timer;
    bool _relay_state;

    unsigned long _last_relay_update = 0;
    unsigned long _relay_update_timer = -1ul;

    void _relay_timer_handler(bool state);

    inline void _relay_write(bool state);

public:
    RelayManager(Timer &timer, uint8_t pin, bool initial_state = RELAY_INITIAL_STATE);

    void update_relay_state(bool state);
};