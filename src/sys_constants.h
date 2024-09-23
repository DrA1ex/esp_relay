#pragma once

#define WIFI_AP_MODE                            WifiMode::AP
#define WIFI_STA_MODE                           WifiMode::STA

#define MAX_RELAY_COUNT                         (4u)

#define MQTT_PREFIX                             ""
#define MQTT_TOPIC_POWER                        MQTT_PREFIX "/power"
#define MQTT_TOPIC_NIGHT_MODE                   MQTT_PREFIX "/night_mode"

#define MQTT_OUT_PREFIX                         MQTT_PREFIX "/out"
#define MQTT_OUT_TOPIC_POWER                    MQTT_OUT_PREFIX "/power"
#define MQTT_OUT_TOPIC_NIGHT_MODE               MQTT_OUT_PREFIX "/night_mode"

#define RESTART_DELAY                           (500u)

#define CONFIG_STRING_SIZE                      (32u)