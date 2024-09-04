#pragma once

#include "sys_constants.h"

#define MDNS_NAME                               "esp_relay"

#define PIN_RELAY                               (4u)

#define RELAY_INITIAL_STATE                     (false)
#define RELAY_SWITCH_INTERVAL                   (3000ul)

#define TIME_ZONE                               (5.f)                   // GMT +5:00