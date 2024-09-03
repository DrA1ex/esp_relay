#pragma once

#include "lib/utils/enum.h"

MAKE_ENUM(ServiceState, uint8_t,
          UNINITIALIZED, 0,
          WIFI_CONNECT, 1,
          INITIALIZING, 2,
          READY, 3
)

MAKE_ENUM(PropertyEnum, uint8_t,
          POWER, 0x00,

          NIGHT_MODE_ENABLED, 0x40,
          NIGHT_MODE_START_TIME, 0x41,
          NIGHT_MODE_END_TIME, 0x42
)

struct __attribute ((packed)) NightModeConfig {
    bool enabled = false;

    uint32_t start_time = 0;
    uint32_t end_time = (uint32_t) 10 * 60 * 60;
};

struct __attribute ((packed)) Config {
    bool power;

    NightModeConfig night_mode;
};

typedef PropertyMetadata<PropertyEnum> AppMetadata;