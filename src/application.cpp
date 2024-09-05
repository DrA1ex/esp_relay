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

        {PacketTypeEnum::RELAY_POWER_0, {
                PropertyEnum::RELAY_POWER_0, PacketTypeEnum::RELAY_POWER_0,
                offsetof(Config, relay[0]), sizeof(Config::relay[0]),
        }},

#if RELAY_COUNT >= 2
        {PacketTypeEnum::RELAY_POWER_1, {
                PropertyEnum::RELAY_POWER_1, PacketTypeEnum::RELAY_POWER_1,
                offsetof(Config, relay[1]), sizeof(Config::relay[1]),
        }},

#endif

#if RELAY_COUNT >= 3
        {PacketTypeEnum::RELAY_POWER_2, {
                PropertyEnum::RELAY_POWER_2, PacketTypeEnum::RELAY_POWER_2,
                offsetof(Config, relay[2]), sizeof(Config::relay[2]),
        }},
#endif

#if RELAY_COUNT >= 4
        {PacketTypeEnum::RELAY_POWER_3, {
                PropertyEnum::RELAY_POWER_3, PacketTypeEnum::RELAY_POWER_3,
                offsetof(Config, relay[3]), sizeof(Config::relay[3]),
        }},
#endif
};

Application::Application(Storage<Config> &config_storage, Timer &timer, NtpTime &ntp_time) :
        AbstractApplication<Config, AppMetadata>(PropToMetaMap),
        _config_storage(config_storage), _timer(timer), ntp_time(ntp_time),
        _night_mode_manager(NightModeManager(ntp_time, timer, config())),
        _relays{
                RelayManager(_timer, PIN_RELAY_0, RELAY_INITIAL_STATE),

#if RELAY_COUNT >= 2
                RelayManager(_timer, PIN_RELAY_1, RELAY_INITIAL_STATE),
#endif

#if RELAY_COUNT >= 3
                RelayManager(_timer, PIN_RELAY_2, RELAY_INITIAL_STATE),
#endif

#if RELAY_COUNT >= 4
                RelayManager(_timer, PIN_RELAY_3, RELAY_INITIAL_STATE),
#endif
        } {}

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
    if constexpr (ACTUAL_RELAY_COUNT == 1) {
        _relays[0].update_relay_state(!_night_mode_manager.active() && config().power);
    } else {
        for (uint8_t i = 0; i < RELAY_COUNT; ++i) {
            auto enabled = config().relay[i] && !_night_mode_manager.active() && config().power;
            _relays[i].update_relay_state(enabled);
        }
    }
}

Response AppPacketHandler::handle_packet_data(uint32_t client_id, const Packet<PacketEnumT> &packet) {
    if (packet.header->type == PacketTypeEnum::GET_CONFIG) {
        return protocol().serialize(app().config());
    }

    return PacketHandler<Application>::handle_packet_data(client_id, packet);
}
