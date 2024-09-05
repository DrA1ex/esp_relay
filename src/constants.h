#pragma once

#include "sys_constants.h"

#define MDNS_NAME                               "esp_relay"

#define RELAY_COUNT                             (1u)                    // Up to 4 relay

#define PIN_RELAY_0                             (4u)
// #define PIN_RELAY_1                             (5u)
// #define PIN_RELAY_2                             (0u)
// #define PIN_RELAY_3                             (2u)

#define RELAY_INITIAL_STATE                     (false)
#define RELAY_SWITCH_INTERVAL                   (3000ul)

#define TIME_ZONE                               (5.f)                   // GMT +5:00