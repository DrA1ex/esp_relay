#include "api.h"

#include "utils/network.h"
#include "utils/math.h"

#include "app/application.h"

ApiWebServer::ApiWebServer(Application &application, const char *path) : _app(application), _path(path) {}

void ApiWebServer::begin(WebServer &server) {
    _on(server, "/status", HTTP_GET, [this](AsyncWebServerRequest *request) {
        response_with_json(request, JsonPropListT{
            {"status", "ok"},
            {"value", (uint8_t) _app.config().power},
        });
    });

    _on(server, "/power", HTTP_GET, [this](AsyncWebServerRequest *request) {
        bool enabled = request->arg("value") == "1";
        _app.set_power(enabled);

        response_with_json_status(request, "ok");
    });

    _on(server, "/debug", HTTP_GET, [](AsyncWebServerRequest *request) {
        char result[64] = {};

        snprintf(result, sizeof(result), "General:\nHeap: %u\nNow: %lu\n",
            ESP.getFreeHeap(), millis());

        request->send_P(200, "text/plain", result);
    });

    _on(server, "/restart", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send_P(200, "text/plain", "OK");

        _app.restart();
    });
}

void ApiWebServer::_on(WebServer &server, const char *uri, WebRequestMethodComposite method, const ArRequestHandlerFunction &onRequest) {
    server.on((_path + uri).c_str(), method, onRequest);
}
