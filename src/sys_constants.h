#pragma once

#define WIFI_AP_MODE                            WifiMode::AP
#define WIFI_STA_MODE                           WifiMode::STA

#define STORAGE_HEADER                          ((uint32_t) 0xd0c1f2c3)
#define STORAGE_CONFIG_VERSION                  ((uint8_t) 1)

#define MQTT_PREFIX                             MDNS_NAME
#define MQTT_TOPIC_POWER                        MQTT_PREFIX "/power"
#define MQTT_TOPIC_NIGHT_MODE                   MQTT_PREFIX "/night_mode"

#define MQTT_OUT_PREFIX                         MQTT_PREFIX "/out"
#define MQTT_OUT_TOPIC_POWER                    MQTT_OUT_PREFIX "/power"
#define MQTT_OUT_TOPIC_NIGHT_MODE               MQTT_OUT_PREFIX "/night_mode"