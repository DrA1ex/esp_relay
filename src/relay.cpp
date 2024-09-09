#include "relay.h"

#include "Arduino.h"

RelayManager::RelayManager(Timer &timer, uint8_t pin) :
        _pin(pin), _timer(timer) {
    pinMode(pin, OUTPUT);
    _relay_write(_relay_state);
}

void RelayManager::update_relay_state(bool state) {
    if (_relay_update_timer != -1ul) {
        D_PRINTF("Relay %u:Clear existing schedule\n", _pin);

        _timer.clear_timeout(_relay_update_timer);
        _relay_update_timer = -1ul;
    }

    if (_relay_state == state) return;

    auto delta = millis() - _last_relay_update;
    if (delta > _switch_interval) {
        D_PRINTF("Relay %u: Change state to %s\n", _pin, state ? "ON" : "OFF");

        _relay_state = state;
        _last_relay_update = millis();
        _relay_write(_relay_state);

        return;
    }

    D_PRINTF("Relay %u: Scheduling state change...\n", _pin);

    _relay_update_timer = _timer.add_timeout(
            [=](auto) {
                _relay_timer_handler(state);
            }, _switch_interval - delta + 1);
}

void RelayManager::_relay_timer_handler(bool state) {
    _relay_update_timer = -1ul;
    update_relay_state(state);
}

void RelayManager::_relay_write(bool state) {
    auto converted_state = state ? _high_level : !_high_level;
    digitalWrite(_pin, converted_state ? HIGH : LOW);
}
