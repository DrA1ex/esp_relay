#include <Arduino.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>

#include "credentials.h"
#include "constants.h"

#include "lib/misc/timer.h"
#include "lib/misc/ntp_time.h"
#include "lib/misc/storage.h"

#include "lib/network/web.h"
#include "lib/network/wifi.h"
#include "lib/network/server/ws.h"
#include "lib/network/server/mqtt.h"


#include "application.h"
#include "type.h"

WifiManager *wifi_manager;

Timer *global_timer;
Storage<Config> *config_storage;
NtpTime *ntp_time;

Application *app;
AppPacketHandler *handler;

WebServer *web_server;
WebSocketServer<Application> *ws_server;
MqttServer<Application> *mqtt_server;

SysConfig *sys_config;

void setup() {
#if defined(DEBUG)
    Serial.begin(74880);
    D_PRINT();

    delay(2000);
#endif

    if (!LittleFS.begin()) {
        D_PRINT("Unable to initialize File System. Rebooting...");

        delay(2000);
        return ESP.restart();
    }

    global_timer = new Timer();
    config_storage = new Storage<Config>(*global_timer, "config");

    config_storage->begin(&LittleFS);

    sys_config = &config_storage->get().sys_config;

    wifi_manager = new WifiManager(sys_config->wifi_ssid, sys_config->wifi_password,
                                   sys_config->wifi_connection_check_interval);

    ntp_time = new NtpTime();
    app = new Application(*config_storage, *global_timer, *ntp_time);
    handler = new AppPacketHandler(*app);

    web_server = new WebServer();
    ws_server = new WebSocketServer<Application>(*app, *handler);
    mqtt_server = new MqttServer(*app);
}

void loop() {
    static ServiceState service_state = ServiceState::UNINITIALIZED;

    switch (service_state) {
        case ServiceState::UNINITIALIZED:
            wifi_manager->connect(sys_config->wifi_mode, sys_config->wifi_max_connection_attempt_interval);

            service_state = ServiceState::WIFI_CONNECT;
            break;

        case ServiceState::WIFI_CONNECT:
            wifi_manager->handle_connection();

            if (wifi_manager->state() == WifiManagerState::CONNECTED) {
                service_state = ServiceState::INITIALIZING;
            }
            break;

        case ServiceState::INITIALIZING:
            ntp_time->begin(sys_config->timezone);
            app->begin();

            web_server->begin(&LittleFS);
            ws_server->begin(*web_server);

            if (sys_config->mqtt) {
                mqtt_server->begin(sys_config->mqtt_host, sys_config->mqtt_port,
                                   sys_config->mqtt_user, sys_config->mqtt_password);
            }

            ArduinoOTA.setHostname(sys_config->mdns_name);
            ArduinoOTA.begin();

            D_PRINTF("ESP Ready: http://%s.local:%u\n", sys_config->mdns_name, web_server->port());

            service_state = ServiceState::READY;
            break;

        case ServiceState::READY:
            wifi_manager->handle_connection();
            ArduinoOTA.handle();

            ntp_time->update();

            global_timer->handle_timers();
            ws_server->handle_incoming_data();
            mqtt_server->handle_connection();
            break;
    }
}