#include "relay.h"

#include "Arduino.h"

RelayManager::RelayManager(Timer &timer, uint8_t pin, bool initial_state) :
        _pin(pin), _timer(timer), _relay_state(initial_state) {
    pinMode(pin, OUTPUT);
    digitalWrite(_pin, initial_state);
}

void RelayManager::update_relay_state(bool state) {
    if (_relay_update_timer != -1ul) {
        D_PRINTF("Relay %u:Clear existing schedule\n", _pin);

        _timer.clear_timeout(_relay_update_timer);
        _relay_update_timer = -1ul;
    }

    if (_relay_state == state) return;

    auto delta = millis() - _last_relay_update;
    if (delta > RELAY_SWITCH_INTERVAL) {
        D_PRINTF("Relay %u: Change state to %s\n", _pin, state ? "ON" : "OFF");

        _relay_state = state;
        _last_relay_update = millis();
        digitalWrite(_pin, _relay_state ? HIGH : LOW);

        return;
    }

    D_PRINTF("Relay %u: Scheduling state change...\n", _pin);

    _relay_update_timer = _timer.add_timeout(
            [=](auto) {
                _relay_timer_handler(state);
            }, RELAY_SWITCH_INTERVAL - delta + 1);
}

void RelayManager::_relay_timer_handler(bool state) {
    _relay_update_timer = -1ul;
    update_relay_state(state);
}
