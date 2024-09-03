#include "application.h"

#include "lib/utils/meta.h"

std::map<PropertyEnum, AppMetadata> Application::PropToMetaMap = {
        define_meta_entry(PropertyEnum::POWER,
                          MQTT_TOPIC_POWER, MQTT_OUT_TOPIC_POWER, &Config::power),
        define_meta_entry(PropertyEnum::NIGHT_MODE_ENABLED,
                          MQTT_TOPIC_NIGHT_MODE, MQTT_OUT_TOPIC_NIGHT_MODE,
                          &Config::night_mode, &NightModeConfig::enabled),
        define_meta_entry(PropertyEnum::NIGHT_MODE_START_TIME, &Config::night_mode, &NightModeConfig::start_time),
        define_meta_entry(PropertyEnum::NIGHT_MODE_END_TIME, &Config::night_mode, &NightModeConfig::end_time),
};

Application::Application(Storage<Config> &config_storage) :
        AbstractApplication<Config, AppMetadata>(PropToMetaMap),
        _config_storage(config_storage) {
}

void Application::begin() {
    event_property_changed().subscribe(this, [this](auto, auto, auto) {
        _config_storage.save();
        load();
    });

    load();
}

void Application::load() {

}