#pragma once

#include <algorithm>

#include "lib/base/parameter.h"
#include "lib/network/wifi.h"
#include "lib/utils/enum.h"

#include "credentials.h"
#include "constants.h"

constexpr uint8_t ACTUAL_RELAY_COUNT = std::max(1, std::min<int>(MAX_RELAY_COUNT, RELAY_COUNT));

MAKE_ENUM(PacketTypeEnum, uint8_t,
          POWER, 0x00,

          RELAY_POWER_0, 0x20,
          RELAY_POWER_1, 0x21,
          RELAY_POWER_2, 0x22,
          RELAY_POWER_3, 0x23,

          NIGHT_MODE_ENABLED, 0x40,
          NIGHT_MODE_START_TIME, 0x41,
          NIGHT_MODE_END_TIME, 0x42,

          SYS_CONFIG_MDNS_NAME, 0x60,
          SYS_CONFIG_WIFI_MODE, 0x61,
          SYS_CONFIG_WIFI_SSID, 0x62,
          SYS_CONFIG_WIFI_PASSWORD, 0x63,
          SYS_CONFIG_WIFI_CONNECTION_CHECK_INTERVAL, 0x64,
          SYS_CONFIG_WIFI_MAX_CONNECTION_ATTEMPT_INTERVAL, 0x65,
          SYS_CONFIG_RELAY_COUNT, 0x66,
          SYS_CONFIG_RELAY_PIN_0, 0x67,
          SYS_CONFIG_RELAY_PIN_1, 0x68,
          SYS_CONFIG_RELAY_PIN_2, 0x69,
          SYS_CONFIG_RELAY_PIN_3, 0x6A,
          SYS_CONFIG_RELAY_HIGH_STATE, 0x6B,
          SYS_CONFIG_RELAY_INITIAL_STATE, 0x6C,
          SYS_CONFIG_RELAY_FORCE_INITIAL_STATE, 0x6D,
          SYS_CONFIG_RELAY_SWITCH_INTERVAL, 0x6E,
          SYS_CONFIG_TIMEZONE, 0x6F,
          SYS_CONFIG_MQTT_ENABLED, 0x70,
          SYS_CONFIG_MQTT_HOST, 0x71,
          SYS_CONFIG_MQTT_PORT, 0x72,
          SYS_CONFIG_MQTT_USER, 0x73,
          SYS_CONFIG_MQTT_PASSWORD, 0x74,

          GET_CONFIG, 0x80,

          RESTART, 0x90,
)

typedef char ConfigString[CONFIG_STRING_SIZE];

struct __attribute ((packed)) SysConfig {
    ConfigString mdns_name{MDNS_NAME};

    WifiMode wifi_mode = WIFI_MODE;
    ConfigString wifi_ssid{WIFI_SSID};
    ConfigString wifi_password{WIFI_PASSWORD};

    uint32_t wifi_connection_check_interval = WIFI_CONNECTION_CHECK_INTERVAL;
    uint32_t wifi_max_connection_attempt_interval = WIFI_MAX_CONNECTION_ATTEMPT_INTERVAL;

    uint8_t relay_count = ACTUAL_RELAY_COUNT;
    uint8_t relay_pin[ACTUAL_RELAY_COUNT]{};

    bool relay_high_state = RELAY_HIGH_LEVEL;
    bool relay_initial_state = RELAY_INITIAL_STATE;
    bool relay_force_initial_state = RELAY_FORCE_INITIAL_STATE;
    uint32_t relay_switch_interval = RELAY_SWITCH_INTERVAL;

    float timezone = TIME_ZONE;

    bool mqtt = MQTT;
    ConfigString mqtt_host{MQTT_HOST};
    uint16_t mqtt_port = MQTT_PORT;
    ConfigString mqtt_user{MQTT_USER};
    ConfigString mqtt_password{MQTT_PASSWORD};

    SysConfig() {
        relay_pin[0] = PIN_RELAY_0;
        if constexpr (ACTUAL_RELAY_COUNT >= 2) relay_pin[1] = PIN_RELAY_1;
        if constexpr (ACTUAL_RELAY_COUNT >= 3) relay_pin[2] = PIN_RELAY_2;
        if constexpr (ACTUAL_RELAY_COUNT >= 4) relay_pin[3] = PIN_RELAY_3;
    }
};

struct __attribute ((packed)) RelayConfig {
    bool power = true;
};

struct __attribute ((packed)) NightModeConfig {
    bool enabled = false;

    uint32_t start_time = 0;
    uint32_t end_time = (uint32_t) 10 * 60 * 60;
};

struct __attribute ((packed)) Config {
    bool power = RELAY_INITIAL_STATE;

    NightModeConfig night_mode{};

    uint8_t relay_count = ACTUAL_RELAY_COUNT;
    RelayConfig relay[ACTUAL_RELAY_COUNT]{};

    SysConfig sys_config{};
};
