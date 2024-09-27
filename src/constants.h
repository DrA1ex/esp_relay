#pragma once

#include "sys_constants.h"

#define WIFI_MODE                               (WIFI_AP_MODE)

#define WIFI_CONNECTION_CHECK_INTERVAL          (5000u)                 // Interval (ms) between Wi-Fi connection check
#define WIFI_MAX_CONNECTION_ATTEMPT_INTERVAL    (120000u)               // Max time (ms) to wait for Wi-Fi connection before switch to AP mode

// 0 - Newer switch to AP mode

#define MDNS_NAME                               "esp_relay"

#define RELAY_COUNT                             (4u)                    // Up to 4 relay

#ifdef ARDUINO_ARCH_ESP32
#define PIN_RELAY_0                             (0u)
#define PIN_RELAY_1                             (1u)
#define PIN_RELAY_2                             (2u)
#define PIN_RELAY_3                             (3u)
#else
#define PIN_RELAY_0                             (2u)
#define PIN_RELAY_1                             (4u)
#define PIN_RELAY_2                             (5u)
#define PIN_RELAY_3                             (14u)
#endif

#define RELAY_HIGH_LEVEL                        (true)                  // Level for opened relay
#define RELAY_INITIAL_STATE                     (false)                 // State for relay on boot
#define RELAY_FORCE_INITIAL_STATE               (false)                 // Forces the relay to a specific initial state upon boot, disregarding any saved value.


#define RELAY_SWITCH_INTERVAL                   (3000ul)                // Minimum interval between relay switching

#define TIME_ZONE                               (5.f)                   // GMT +5:00

#define MQTT                                    (0u)

#include "_override/credentials.h"
