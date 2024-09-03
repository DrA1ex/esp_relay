#include <Arduino.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>

#include "constants.h"

#include "lib/misc/timer.h"
#include "lib/misc/storage.h"

#include "lib/network/web.h"
#include "lib/network/wifi.h"
#include "lib/network/server/ws.h"
#include "lib/network/server/mqtt.h"


#include "application.h"
#include "credentials.h"
#include "type.h"

WifiManager *wifi_manager;

Timer *global_timer;
Storage<Config> *config_storage;

Application *app;
AppPacketHandler *handler;

WebServer *web_server;
WebSocketServer<Application> *ws_server;
MqttServer<Application> *mqtt_server;

void setup() {
#if defined(DEBUG)
    Serial.begin(74880);
    D_PRINT();

    delay(2000);
#endif

    pinMode(PIN_RELAY, OUTPUT);

    if (!LittleFS.begin()) {
        D_PRINT("Unable to initialize FS");
    }

    wifi_manager = new WifiManager(WIFI_SSID, WIFI_PASSWORD);

    global_timer = new Timer();
    config_storage = new Storage<Config>(*global_timer, "config", STORAGE_CONFIG_VERSION, STORAGE_HEADER);

    config_storage->begin(&LittleFS);

    app = new Application(*config_storage, *global_timer);
    handler = new AppPacketHandler(*app);

    web_server = new WebServer();
    ws_server = new WebSocketServer<Application>(*app, *handler);
    mqtt_server = new MqttServer(*app);
}

void loop() {
    static ServiceState service_state = ServiceState::UNINITIALIZED;

    switch (service_state) {
        case ServiceState::UNINITIALIZED:
            wifi_manager->connect(WifiMode::STA);

            service_state = ServiceState::WIFI_CONNECT;
            break;

        case ServiceState::WIFI_CONNECT:
            wifi_manager->handle_connection();

            if (wifi_manager->state() == WifiManagerState::CONNECTED) {
                service_state = ServiceState::INITIALIZING;
            }
            break;

        case ServiceState::INITIALIZING:
            app->begin();

            web_server->begin(&LittleFS);
            ws_server->begin(*web_server);
            mqtt_server->begin(MQTT_HOST, MQTT_PORT, MQTT_USER, MQTT_PASSWORD);

            ArduinoOTA.setHostname(MDNS_NAME);
            ArduinoOTA.begin();

            service_state = ServiceState::READY;
            break;

        case ServiceState::READY:
            wifi_manager->handle_connection();
            ArduinoOTA.handle();

            global_timer->handle_timers();
            ws_server->handle_incoming_data();
            mqtt_server->handle_connection();

            break;
    }
}