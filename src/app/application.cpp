#include "application.h"

void Application::begin() {
    D_PRINT("Starting application...");

    if (!LittleFS.begin()) {
        D_PRINT("Unable to initialize FS");
    }

    _bootstrap = std::make_unique<Bootstrap<Config, PacketTypeEnum>>(&LittleFS);

    auto &sys_config = _bootstrap->config().sys_config;
    _bootstrap->begin({
        .mdns_name = sys_config.mdns_name,
        .wifi_mode = sys_config.wifi_mode,
        .wifi_ssid = sys_config.wifi_ssid,
        .wifi_password = sys_config.wifi_password,
        .wifi_connection_timeout = sys_config.wifi_max_connection_attempt_interval,
        .mqtt_enabled = sys_config.mqtt,
        .mqtt_host = sys_config.mqtt_host,
        .mqtt_port = sys_config.mqtt_port,
        .mqtt_user = sys_config.mqtt_user,
        .mqtt_password = sys_config.mqtt_password,
    });

    Timer &timer = _bootstrap->timer();

    _ntp_time = std::make_unique<NtpTime>();
    _night_mode_manager = std::make_unique<NightModeManager>(*_ntp_time, timer, _bootstrap->config());

    _relays = std::make_unique<std::array<std::unique_ptr<RelayManager>, ACTUAL_RELAY_COUNT>>();
    _relay_count = std::min(config().relay_count, ACTUAL_RELAY_COUNT);

    (*_relays)[0] = std::make_unique<RelayManager>(timer, sys_config.relay_pin[0]);
    if (_relay_count >= 2) (*_relays)[1] = std::make_unique<RelayManager>(timer, sys_config.relay_pin[1]);
    if (_relay_count >= 3) (*_relays)[2] = std::make_unique<RelayManager>(timer, sys_config.relay_pin[2]);
    if (_relay_count >= 4) (*_relays)[3] = std::make_unique<RelayManager>(timer, sys_config.relay_pin[3]);

    for (auto &relay: *_relays) {
        relay->set_initial_state(sys_config.relay_initial_state);
        relay->set_high_level(sys_config.relay_high_state);
        relay->set_switch_interval(sys_config.relay_switch_interval);
    }

    if (sys_config.relay_force_initial_state) {
        config().power = sys_config.relay_initial_state;
    }

    _night_mode_manager->event_night_mode().subscribe(this, [this](auto, auto state, auto) {
        load();
    });

    _bootstrap->event_state_changed().subscribe(this, BootstrapState::READY, [this, &sys_config](auto, auto, auto) {
        _ntp_time->begin(sys_config.timezone);

        _ntp_time->update();
        _night_mode_manager->update();
        load();

        _bootstrap->timer().add_interval([this](auto) {
            if (_bootstrap->wifi_manager()->mode() == WifiMode::STA) {
                _ntp_time->update();
            }
        }, BOOTSTRAP_SERVICE_LOOP_INTERVAL);
    });

    _api = std::make_unique<ApiWebServer>(*this);
    _api->begin(_bootstrap->web_server());

    load();
    _setup();
}

void Application::event_loop() {
    _bootstrap->event_loop();
}

void Application::load() {
    auto &relays = *_relays;
    for (uint8_t i = 0; i < _relay_count; ++i) {
        auto enabled = config().relay[i].power && !_night_mode_manager->active() && config().power;
        relays[i]->update_relay_state(enabled);
    }
}

void Application::update() {
    _bootstrap->save_changes();
    load();
}

void Application::set_power(bool value) {
    config().power = value;
    update();

    NotificationBus::get().notify_parameter_changed(this, _metadata->power.get_parameter());
}

void Application::_setup() {
    NotificationBus::get().subscribe([this](auto sender, auto param) {
        if (sender != this) _handle_property_change(param);
    });

    auto &ws_server = _bootstrap->ws_server();
    auto &mqtt_server = _bootstrap->mqtt_server();

    _metadata = std::make_unique<ConfigMeta>(build_metadata(config()));
    _metadata->visit([this, &ws_server, &mqtt_server](AbstractPropertyMeta *meta) {
        auto binary_protocol = (BinaryProtocolMeta<PacketTypeEnum> *) meta->get_binary_protocol();
        if (binary_protocol->packet_type.has_value()) {
            ws_server->register_parameter(*binary_protocol->packet_type, meta->get_parameter());
            VERBOSE(D_PRINTF("WebSocket: Register property %s\r\n", __debug_enum_str(*binary_protocol->packet_type)));
        }

        auto mqtt_protocol = meta->get_mqtt_protocol();
        if (mqtt_protocol->topic_in && mqtt_protocol->topic_out) {
            mqtt_server->register_parameter(mqtt_protocol->topic_in, mqtt_protocol->topic_out, meta->get_parameter());
            VERBOSE(D_PRINTF("MQTT: Register property %s <-> %s\r\n", mqtt_protocol->topic_in, mqtt_protocol->topic_out));
        } else if (mqtt_protocol->topic_out) {
            mqtt_server->register_notification(mqtt_protocol->topic_out, meta->get_parameter());
            VERBOSE(D_PRINTF("MQTT: Register notification -> %s\r\n", mqtt_protocol->topic_out));
        }

        if (binary_protocol->packet_type.has_value()) {
            _parameter_to_packet[meta->get_parameter()] = binary_protocol->packet_type.value();
        }
    });

    ws_server->register_data_request(PacketTypeEnum::GET_CONFIG, _metadata->data.config);
    ws_server->register_command(PacketTypeEnum::RESTART, [this] { _bootstrap->restart(); });
}

void Application::_handle_property_change(const AbstractParameter *param) {
    auto it = _parameter_to_packet.find(param);
    if (it == _parameter_to_packet.end()) return;

    auto type = it->second;
    if (type >= PacketTypeEnum::NIGHT_MODE_ENABLED && type <= PacketTypeEnum::NIGHT_MODE_END_TIME) {
        _night_mode_manager->update();
    }

    update();
}
