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


        define_meta_entry(PropertyEnum::SYS_CONFIG_MDNS_NAME, PacketTypeEnum::SYS_CONFIG_MDNS_NAME,
                          &Config::sys_config, &SysConfig::mdns_name),

        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_MODE, PacketTypeEnum::SYS_CONFIG_WIFI_MODE,
                          &Config::sys_config, &SysConfig::wifi_mode),

        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_SSID, PacketTypeEnum::SYS_CONFIG_WIFI_SSID,
                          &Config::sys_config, &SysConfig::wifi_ssid),

        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_PASSWORD, PacketTypeEnum::SYS_CONFIG_WIFI_PASSWORD,
                          &Config::sys_config, &SysConfig::wifi_password),

        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_CONNECTION_CHECK_INTERVAL,
                          PacketTypeEnum::SYS_CONFIG_WIFI_CONNECTION_CHECK_INTERVAL,
                          &Config::sys_config, &SysConfig::wifi_connection_check_interval),

        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_MAX_CONNECTION_ATTEMPT_INTERVAL,
                          PacketTypeEnum::SYS_CONFIG_WIFI_MAX_CONNECTION_ATTEMPT_INTERVAL,
                          &Config::sys_config, &SysConfig::wifi_max_connection_attempt_interval),

        define_meta_entry(PropertyEnum::SYS_CONFIG_RELAY_COUNT, PacketTypeEnum::SYS_CONFIG_RELAY_COUNT,
                          &Config::sys_config, &SysConfig::relay_count),

        define_meta_entry(PropertyEnum::SYS_CONFIG_RELAY_HIGH_STATE, PacketTypeEnum::SYS_CONFIG_RELAY_HIGH_STATE,
                          &Config::sys_config, &SysConfig::relay_high_state),

        define_meta_entry(PropertyEnum::SYS_CONFIG_RELAY_INITIAL_STATE, PacketTypeEnum::SYS_CONFIG_RELAY_INITIAL_STATE,
                          &Config::sys_config, &SysConfig::relay_initial_state),

        define_meta_entry(PropertyEnum::SYS_CONFIG_RELAY_FORCE_INITIAL_STATE, PacketTypeEnum::SYS_CONFIG_RELAY_FORCE_INITIAL_STATE,
                          &Config::sys_config, &SysConfig::relay_force_initial_state),

        define_meta_entry(PropertyEnum::SYS_CONFIG_RELAY_SWITCH_INTERVAL, PacketTypeEnum::SYS_CONFIG_RELAY_SWITCH_INTERVAL,
                          &Config::sys_config, &SysConfig::relay_switch_interval),

        define_meta_entry(PropertyEnum::SYS_CONFIG_TIMEZONE, PacketTypeEnum::SYS_CONFIG_TIMEZONE,
                          &Config::sys_config, &SysConfig::timezone),

        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_ENABLED, PacketTypeEnum::SYS_CONFIG_MQTT_ENABLED,
                          &Config::sys_config, &SysConfig::mqtt),

        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_HOST, PacketTypeEnum::SYS_CONFIG_MQTT_HOST,
                          &Config::sys_config, &SysConfig::mqtt_host),

        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_PORT, PacketTypeEnum::SYS_CONFIG_MQTT_PORT,
                          &Config::sys_config, &SysConfig::mqtt_port),

        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_USER, PacketTypeEnum::SYS_CONFIG_MQTT_USER,
                          &Config::sys_config, &SysConfig::mqtt_user),

        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_PASSWORD, PacketTypeEnum::SYS_CONFIG_MQTT_PASSWORD,
                          &Config::sys_config, &SysConfig::mqtt_password),


        {
                PacketTypeEnum::RELAY_POWER_0, {
                PropertyEnum::RELAY_POWER_0, PacketTypeEnum::RELAY_POWER_0,
                offsetof(Config, relay[0].power), sizeof(Config::relay[0].power),
        }
        },
        {
                PacketTypeEnum::SYS_CONFIG_RELAY_PIN_0, {
                PropertyEnum::SYS_CONFIG_RELAY_PIN_0, PacketTypeEnum::SYS_CONFIG_RELAY_PIN_0,
                offsetof(Config, sys_config.relay_pin[0]), sizeof(Config::sys_config.relay_pin[0]),
        }
        },


#if RELAY_COUNT >= 2
        {
                PacketTypeEnum::RELAY_POWER_1, {
                PropertyEnum::RELAY_POWER_1, PacketTypeEnum::RELAY_POWER_1,
                offsetof(Config, relay[1].power), sizeof(Config::relay[1].power),
        }
        },
        {
                PacketTypeEnum::SYS_CONFIG_RELAY_PIN_1, {
                PropertyEnum::SYS_CONFIG_RELAY_PIN_1, PacketTypeEnum::SYS_CONFIG_RELAY_PIN_1,
                offsetof(Config, sys_config.relay_pin[1]), sizeof(Config::sys_config.relay_pin[1]),
        }
        },
#endif

#if RELAY_COUNT >= 3
        {
                PacketTypeEnum::RELAY_POWER_2, {
                PropertyEnum::RELAY_POWER_2, PacketTypeEnum::RELAY_POWER_2,
                offsetof(Config, relay[2].power), sizeof(Config::relay[2].power),
        }
        },
        {
                PacketTypeEnum::SYS_CONFIG_RELAY_PIN_2, {
                PropertyEnum::SYS_CONFIG_RELAY_PIN_2, PacketTypeEnum::SYS_CONFIG_RELAY_PIN_2,
                offsetof(Config, sys_config.relay_pin[2]), sizeof(Config::sys_config.relay_pin[2]),
        }
        },
#endif

#if RELAY_COUNT >= 4
        {
                PacketTypeEnum::RELAY_POWER_3, {
                PropertyEnum::RELAY_POWER_3, PacketTypeEnum::RELAY_POWER_3,
                offsetof(Config, relay[3].power), sizeof(Config::relay[3].power),
        }
        },
        {
                PacketTypeEnum::SYS_CONFIG_RELAY_PIN_3, {
                PropertyEnum::SYS_CONFIG_RELAY_PIN_3, PacketTypeEnum::SYS_CONFIG_RELAY_PIN_3,
                offsetof(Config, sys_config.relay_pin[3]), sizeof(Config::sys_config.relay_pin[3]),
        }
        },
#endif
};

Application::Application(Storage<Config> &config_storage, Timer &timer, NtpTime &ntp_time) :
        AbstractApplication<Config, AppMetadata>(PropToMetaMap),
        _config_storage(config_storage), _timer(timer),
        _night_mode_manager(NightModeManager(ntp_time, timer, config())),
        _relays{ //TODO:
                RelayManager(_timer, sys_config().relay_pin[0]),
#if RELAY_COUNT >= 2
                RelayManager(_timer, sys_config().relay_pin[1]),
#endif
#if RELAY_COUNT >= 3
                RelayManager(_timer, sys_config().relay_pin[2]),
#endif
#if RELAY_COUNT >= 4
                RelayManager(_timer, sys_config().relay_pin[3]),
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

    if (sys_config().relay_force_initial_state) {
        config().power = sys_config().relay_initial_state;
    }

    _night_mode_manager.update();
    load();
}

void Application::load() {
    if constexpr (ACTUAL_RELAY_COUNT == 1) {
        _relays[0].update_relay_state(!_night_mode_manager.active() && config().power);
    } else {
        for (uint8_t i = 0; i < ACTUAL_RELAY_COUNT; ++i) {
            auto enabled = config().relay[i].power && !_night_mode_manager.active() && config().power;
            _relays[i].update_relay_state(enabled);
        }
    }
}

void Application::restart() {
    D_PRINTF("Received restart signal. Restarting after %u ms.\n", RESTART_DELAY);

    if (_config_storage.is_pending_commit()) _config_storage.force_save();

    _timer.add_timeout([](auto) { EspClass::restart(); }, RESTART_DELAY);
}

Response AppPacketHandler::handle_packet_data(uint32_t client_id, const Packet<PacketEnumT> &packet) {
    if (packet.header->type == PacketTypeEnum::GET_CONFIG) {
        return protocol().serialize(app().config());
    } else if (packet.header->type == PacketTypeEnum::RESTART) {
        app().restart();
        return Response::ok();
    }

    return PacketHandler<Application>::handle_packet_data(client_id, packet);
}
