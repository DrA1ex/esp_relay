#include "application.h"

#include "lib/utils/meta.h"

std::map<PacketTypeEnum, AppMetadata> Application::PropToMetaMap = {
        define_meta_entry(PropertyEnum::POWER, PacketTypeEnum::POWER,
                          MQTT_TOPIC_POWER, MQTT_OUT_TOPIC_POWER, &Config::power),

        define_meta_entry(PropertyEnum::NIGHT_MODE_ENABLED, PacketTypeEnum::NIGHT_MODE_ENABLED,
                          MQTT_TOPIC_NIGHT_MODE, MQTT_OUT_TOPIC_NIGHT_MODE,
                          &Config::night_mode, &NightModeConfig::enabled),

        define_meta_entry(PropertyEnum::NIGHT_MODE_START_TIME, PacketTypeEnum::NIGHT_MODE_START_TIME,
                          &Config::night_mode, &NightModeConfig::start_time),

        define_meta_entry(PropertyEnum::NIGHT_MODE_END_TIME, PacketTypeEnum::NIGHT_MODE_END_TIME,
                          &Config::night_mode, &NightModeConfig::end_time),
};

Application::Application(Storage<Config> &config_storage, Timer &timer, NtpTime &ntp_time) :
        AbstractApplication<Config, AppMetadata>(PropToMetaMap),
        _config_storage(config_storage), _timer(timer), ntp_time(ntp_time),
        _night_mode_manager(NightModeManager(ntp_time, timer, config())) {
}

void Application::begin() {
    event_property_changed().subscribe(this, [this](auto, auto prop, auto) {
        _config_storage.save();

        if (prop >= PropertyEnum::NIGHT_MODE_ENABLED && prop <= PropertyEnum::NIGHT_MODE_END_TIME) {
            _night_mode_manager.update();
        }

        load();
    });

    _night_mode_manager.event_night_mode().subscribe(this, [this](auto, auto state, auto) {
        load();
    });

    _night_mode_manager.update();
    load();
}

void Application::load() {
    update_relay_state(!_night_mode_manager.active() && config().power);
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
                _relay_timer_handler(state);
            }, RELAY_SWITCH_INTERVAL - delta + 1);
}

void Application::_relay_timer_handler(bool state) {
    _relay_update_timer = -1ul;
    update_relay_state(state);
}

Response AppPacketHandler::handle_packet_data(uint32_t client_id, const Packet<PacketEnumT> &packet) {
    if (packet.header->type == PacketTypeEnum::GET_CONFIG) {
        return protocol().serialize(app().config());
    }

    return PacketHandler<Application>::handle_packet_data(client_id, packet);
}
