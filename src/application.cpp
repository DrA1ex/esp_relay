#include "application.h"

#include <memory>

#include "lib/utils/meta.h"

struct RelayStateChangeArg {
    Application *app;
    bool state;
};

std::map<PropertyEnum, AppMetadata> Application::PropToMetaMap = {
        define_meta_entry(PropertyEnum::POWER,
                          MQTT_TOPIC_POWER, MQTT_OUT_TOPIC_POWER, &Config::power),
        define_meta_entry(PropertyEnum::NIGHT_MODE_ENABLED,
                          MQTT_TOPIC_NIGHT_MODE, MQTT_OUT_TOPIC_NIGHT_MODE,
                          &Config::night_mode, &NightModeConfig::enabled),
        define_meta_entry(PropertyEnum::NIGHT_MODE_START_TIME, &Config::night_mode, &NightModeConfig::start_time),
        define_meta_entry(PropertyEnum::NIGHT_MODE_END_TIME, &Config::night_mode, &NightModeConfig::end_time),
};

Application::Application(Storage<Config> &config_storage, Timer &timer) :
        AbstractApplication<Config, AppMetadata>(PropToMetaMap),
        _config_storage(config_storage), _timer(timer) {
}

void Application::begin() {
    event_property_changed().subscribe(this, [this](auto, auto, auto) {
        _config_storage.save();
        load();
    });

    load();
}

void Application::load() {
    update_relay_state(config().power);
}

void Application::update_relay_state(bool state) {
    if (_relay_update_timer != -1ul) {
        D_PRINT("Clear existing relay schedule");

        _timer.clear_timeout(_relay_update_timer);
        _relay_update_timer = -1ul;
    }

    if (_relay_state == state) return;

    auto delta = millis() - _last_relay_update;
    if (delta > RELAY_SWITCH_INTERVAL) {
        D_PRINTF("Change relay state to %s\n", state ? "ON" : "OFF");

        _relay_state = state;
        _last_relay_update = millis();
        digitalWrite(PIN_RELAY, _relay_state ? HIGH : LOW);

        return;
    }

    D_PRINT("Scheduling relay state change...");

    _relay_update_timer = _timer.add_timeout(
            [=](auto) {
                _relay_update_timer = -1ul;
                this->update_relay_state(state);
            }, RELAY_SWITCH_INTERVAL - delta + 1);
}