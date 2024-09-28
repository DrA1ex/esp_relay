#pragma once

#include <cstdint>

#include "lib/debug.h"
#include "lib/misc/timer.h"

#include "constants.h"

class RelayManager {
    uint8_t _pin;
    Timer &_timer;

    bool _initalized = false;
    bool _relay_state = RELAY_INITIAL_STATE;
    bool _high_level = RELAY_HIGH_LEVEL;
    uint32_t _switch_interval = RELAY_SWITCH_INTERVAL;

    unsigned long _last_relay_update = 0;
    unsigned long _relay_update_timer = -1ul;

    void _relay_timer_handler(bool state);

    inline void _relay_write(bool state);

public:
    RelayManager(Timer &timer, uint8_t pin);

    void begin();
    void update_relay_state(bool state);

    inline void set_initial_state(bool value) { _relay_state = value; }
    inline void set_high_level(bool value) { _high_level = value; }
    inline void set_switch_interval(uint32_t value) { _switch_interval = value; }
};