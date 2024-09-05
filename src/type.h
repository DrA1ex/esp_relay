#pragma once

#include <algorithm>

#include "lib/base/application.h"
#include "lib/utils/enum.h"

#include "constants.h"

constexpr uint8_t MAX_RELAY_COUNT = 4;
constexpr uint8_t ACTUAL_RELAY_COUNT = std::max(1, std::min<int>(MAX_RELAY_COUNT, RELAY_COUNT));

MAKE_ENUM(ServiceState, uint8_t,
          UNINITIALIZED, 0,
          WIFI_CONNECT, 1,
          INITIALIZING, 2,
          READY, 3
)

MAKE_ENUM(PropertyEnum, uint8_t,
          POWER, 0,

          RELAY_POWER_0, 10,
          RELAY_POWER_1, 11,
          RELAY_POWER_2, 12,
          RELAY_POWER_3, 13,

          NIGHT_MODE_ENABLED, 20,
          NIGHT_MODE_START_TIME, 21,
          NIGHT_MODE_END_TIME, 22
)

MAKE_ENUM(PacketTypeEnum, uint8_t,
          POWER, 0x00,

          RELAY_POWER_0, 0x20,
          RELAY_POWER_1, 0x21,
          RELAY_POWER_2, 0x22,
          RELAY_POWER_3, 0x23,

          NIGHT_MODE_ENABLED, 0x40,
          NIGHT_MODE_START_TIME, 0x41,
          NIGHT_MODE_END_TIME, 0x42,

          GET_CONFIG, 0x80,
)

struct __attribute ((packed)) RelayConfig {
    bool power = RELAY_INITIAL_STATE;
};

struct __attribute ((packed)) NightModeConfig {
    bool enabled = false;

    uint32_t start_time = 0;
    uint32_t end_time = (uint32_t) 10 * 60 * 60;
};

struct __attribute ((packed)) Config {
    bool power = true;

    NightModeConfig night_mode;

    uint8_t relay_count = ACTUAL_RELAY_COUNT;
    bool relay[ACTUAL_RELAY_COUNT]{};
};

typedef PropertyMetadata<PropertyEnum, PacketTypeEnum> AppMetadata;