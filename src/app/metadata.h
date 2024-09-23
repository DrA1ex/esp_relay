#pragma once

#include <lib/base/parameter.h>
#include <lib/utils/metadata.h>

#include "type.h"

DECLARE_META_TYPE(AppMetaProperty, PacketTypeEnum)

DECLARE_META(RelayConfigMeta, AppMetaProperty,
             MEMBER(Parameter<bool>, power)
)

DECLARE_META(NightModeConfigMeta, AppMetaProperty,
             MEMBER(Parameter<bool>, enabled),
             MEMBER(Parameter<uint32_t>, start_time),
             MEMBER(Parameter<uint32_t>, end_time)
)

DECLARE_META(SysConfigMeta, AppMetaProperty,
             MEMBER(Parameter<uint8_t>, relay_count),
             MEMBER_ARRAY(Parameter<uint8_t>, relay_pin, ACTUAL_RELAY_COUNT),
             MEMBER(Parameter<bool>, relay_high_state),
             MEMBER(Parameter<bool>, relay_initial_state),
             MEMBER(Parameter<bool>, relay_force_initial_state),
             MEMBER(Parameter<uint32_t>, relay_switch_interval),
             MEMBER(FixedString, mdns_name),
             MEMBER(Parameter<uint8_t>, wifi_mode),
             MEMBER(FixedString, wifi_ssid),
             MEMBER(FixedString, wifi_password),
             MEMBER(Parameter<uint32_t>, wifi_connection_check_interval),
             MEMBER(Parameter<uint32_t>, wifi_max_connection_attempt_interval),
             MEMBER(Parameter<float>, timezone),
             MEMBER(Parameter<bool>, mqtt),
             MEMBER(FixedString, mqtt_host),
             MEMBER(Parameter<uint16_t>, mqtt_port),
             MEMBER(FixedString, mqtt_user),
             MEMBER(FixedString, mqtt_password)
)

DECLARE_META(DataConfigMeta, AppMetaProperty,
             MEMBER(ComplexParameter<Config>, config),
)

DECLARE_META(ConfigMeta, AppMetaProperty,
             MEMBER(Parameter<bool>, power),
             SUB_TYPE(NightModeConfigMeta, night_mode),
             SUB_TYPE_ARRAY(RelayConfigMeta, relay, ACTUAL_RELAY_COUNT),
             SUB_TYPE(SysConfigMeta, sys_config),

             SUB_TYPE(DataConfigMeta, data)
)


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-packed-member"
inline ConfigMeta build_metadata(Config &config) {
    return {
            .power = {
                    PacketTypeEnum::POWER,
                    MQTT_TOPIC_POWER, MQTT_OUT_TOPIC_POWER,
                    &config.power
            },
            .night_mode = {
                    .enabled = {
                            PacketTypeEnum::NIGHT_MODE_ENABLED,
                            MQTT_TOPIC_NIGHT_MODE, MQTT_OUT_TOPIC_NIGHT_MODE,
                            &config.night_mode.enabled
                    },
                    .start_time = {
                            PacketTypeEnum::NIGHT_MODE_START_TIME,
                            &config.night_mode.start_time
                    },
                    .end_time = {
                            PacketTypeEnum::NIGHT_MODE_END_TIME,
                            &config.night_mode.end_time
                    }
            },
            .relay = {
                    {.power = {PacketTypeEnum::RELAY_POWER_0, &config.relay[0].power}},
#if RELAY_COUNT >= 2
                    {.power = {PacketTypeEnum::RELAY_POWER_1, &config.relay[1].power}},
#endif
#if RELAY_COUNT >= 3
                    {.power = {PacketTypeEnum::RELAY_POWER_2, &config.relay[2].power}},
#endif
#if RELAY_COUNT >= 4
                    {.power = {PacketTypeEnum::RELAY_POWER_3, &config.relay[3].power}},
#endif
            },
            .sys_config = {
                    .relay_count = {
                            PacketTypeEnum::SYS_CONFIG_RELAY_COUNT,
                            &config.sys_config.relay_count
                    },
                    .relay_pin = {
                            {PacketTypeEnum::SYS_CONFIG_RELAY_PIN_0, &config.sys_config.relay_pin[0]},
#if RELAY_COUNT >= 2
                            {PacketTypeEnum::SYS_CONFIG_RELAY_PIN_1, &config.sys_config.relay_pin[1]},
#endif
#if RELAY_COUNT >= 3
                            {PacketTypeEnum::SYS_CONFIG_RELAY_PIN_2, &config.sys_config.relay_pin[2]},
#endif
#if RELAY_COUNT >= 4
                            {PacketTypeEnum::SYS_CONFIG_RELAY_PIN_3, &config.sys_config.relay_pin[3]},
#endif
                    },
                    .relay_high_state = {
                            PacketTypeEnum::SYS_CONFIG_RELAY_HIGH_STATE,
                            &config.sys_config.relay_high_state
                    },
                    .relay_initial_state = {
                            PacketTypeEnum::SYS_CONFIG_RELAY_INITIAL_STATE,
                            &config.sys_config.relay_initial_state
                    },
                    .relay_force_initial_state = {
                            PacketTypeEnum::SYS_CONFIG_RELAY_FORCE_INITIAL_STATE,
                            &config.sys_config.relay_force_initial_state
                    },
                    .relay_switch_interval = {
                            PacketTypeEnum::SYS_CONFIG_RELAY_SWITCH_INTERVAL,
                            &config.sys_config.relay_switch_interval
                    },

                    .mdns_name = {
                            PacketTypeEnum::SYS_CONFIG_MDNS_NAME,
                            {config.sys_config.mdns_name, CONFIG_STRING_SIZE}
                    },
                    .wifi_mode = {
                            PacketTypeEnum::SYS_CONFIG_WIFI_MODE,
                            (uint8_t *) &config.sys_config.wifi_mode
                    },
                    .wifi_ssid = {
                            PacketTypeEnum::SYS_CONFIG_WIFI_SSID,
                            {config.sys_config.wifi_ssid, CONFIG_STRING_SIZE}
                    },
                    .wifi_password = {
                            PacketTypeEnum::SYS_CONFIG_WIFI_PASSWORD,
                            {config.sys_config.wifi_password, CONFIG_STRING_SIZE}
                    },
                    .wifi_connection_check_interval = {
                            PacketTypeEnum::SYS_CONFIG_WIFI_CONNECTION_CHECK_INTERVAL,
                            &config.sys_config.wifi_connection_check_interval
                    },
                    .wifi_max_connection_attempt_interval = {
                            PacketTypeEnum::SYS_CONFIG_WIFI_MAX_CONNECTION_ATTEMPT_INTERVAL,
                            &config.sys_config.wifi_max_connection_attempt_interval
                    },
                    .timezone = {
                            PacketTypeEnum::SYS_CONFIG_TIMEZONE,
                            &config.sys_config.timezone
                    },
                    .mqtt = {
                            PacketTypeEnum::SYS_CONFIG_MQTT_ENABLED,
                            &config.sys_config.mqtt
                    },
                    .mqtt_host = {
                            PacketTypeEnum::SYS_CONFIG_MQTT_HOST,
                            {config.sys_config.mqtt_host, CONFIG_STRING_SIZE}
                    },
                    .mqtt_port = {
                            PacketTypeEnum::SYS_CONFIG_MQTT_PORT,
                            &config.sys_config.mqtt_port
                    },
                    .mqtt_user = {
                            PacketTypeEnum::SYS_CONFIG_MQTT_USER,
                            {config.sys_config.mqtt_user, CONFIG_STRING_SIZE}
                    },
                    .mqtt_password = {
                            PacketTypeEnum::SYS_CONFIG_MQTT_PASSWORD,
                            {config.sys_config.mqtt_password, CONFIG_STRING_SIZE}
                    }
            },

            .data {
                    .config = ComplexParameter(&config),
            },
    };
}
#pragma clang diagnostic pop