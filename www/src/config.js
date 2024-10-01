import {AppConfigBase} from "./lib/index.js";

export const DEFAULT_ADDRESS = "esp_relay.local";


export const PacketType = {
    POWER: 0x00,

    RELAY_POWER_0: 0x20,
    RELAY_POWER_1: 0x21,
    RELAY_POWER_2: 0x22,
    RELAY_POWER_3: 0x23,

    NIGHT_MODE_ENABLED: 0x40,
    NIGHT_MODE_START_TIME: 0x41,
    NIGHT_MODE_END_TIME: 0x42,

    SYS_CONFIG_MDNS_NAME: 0x60,
    SYS_CONFIG_WIFI_MODE: 0x61,
    SYS_CONFIG_WIFI_SSID: 0x62,
    SYS_CONFIG_WIFI_PASSWORD: 0x63,
    SYS_CONFIG_WIFI_CONNECTION_CHECK_INTERVAL: 0x64,
    SYS_CONFIG_WIFI_MAX_CONNECTION_ATTEMPT_INTERVAL: 0x65,
    SYS_CONFIG_RELAY_COUNT: 0x66,
    SYS_CONFIG_RELAY_PIN_0: 0x67,
    SYS_CONFIG_RELAY_PIN_1: 0x68,
    SYS_CONFIG_RELAY_PIN_2: 0x69,
    SYS_CONFIG_RELAY_PIN_3: 0x6A,
    SYS_CONFIG_RELAY_HIGH_STATE: 0x6B,
    SYS_CONFIG_RELAY_INITIAL_STATE: 0x6C,
    SYS_CONFIG_RELAY_FORCE_INITIAL_STATE: 0x6D,
    SYS_CONFIG_RELAY_SWITCH_INTERVAL: 0x6E,
    SYS_CONFIG_TIMEZONE: 0x6F,
    SYS_CONFIG_MQTT_ENABLED: 0x70,
    SYS_CONFIG_MQTT_HOST: 0x71,
    SYS_CONFIG_MQTT_PORT: 0x72,
    SYS_CONFIG_MQTT_USER: 0x73,
    SYS_CONFIG_MQTT_PASSWORD: 0x74,

    GET_CONFIG: 0x80,
    RESTART: 0x90,
}

export class Config extends AppConfigBase {
    power;
    night_mode;
    relay;

    sys_config;

    _relay_cfg;

    get cmd() {
        return PacketType.GET_CONFIG;
    }

    constructor(props) {
        super(props);

        this.lists["wifi_mode"] = [
            {code: 0, name: "AP"},
            {code: 1, name: "STA"},
        ]
    }


    parse(parser) {
        this.power = parser.readBoolean();
        this.night_mode = {
            enabled: parser.readBoolean(),
            startTime: parser.readUint32(),
            endTime: parser.readUint32()
        };

        const relayCount = parser.readUint8();
        this.relay = new Array(relayCount);
        for (let i = 0; i < relayCount; i++) {
            this.relay[i] = {
                index: i,
                power: parser.readBoolean()
            };
        }

        this.sys_config = {
            mdns_name: parser.readFixedString(32),

            wifi_mode: parser.readUint8(),
            wifi_ssid: parser.readFixedString(32),
            wifi_password: parser.readFixedString(32),
            wifi_connection_check_interval: parser.readUint32(),
            wifi_max_connection_attempt_interval: parser.readUint32(),

            relay_count: parser.readUint8(),
            relay: Array.from({length: relayCount}, () => ({pin: parser.readUint8()})),
            relay_high_state: parser.readBoolean(),
            relay_initial_state: parser.readBoolean(),
            relay_force_initial_state: parser.readBoolean(),
            relay_switch_interval: parser.readUint32(),

            timezone: parser.readFloat32(),

            mqtt: parser.readBoolean(),
            mqtt_host: parser.readFixedString(32),
            mqtt_port: parser.readUint16(),
            mqtt_user: parser.readFixedString(32),
            mqtt_password: parser.readFixedString(32)
        }

        this.refreshRelayConfig();
    }

    refreshRelayConfig() {
        this._relay_cfg = Array.from({length: this.sys_config.relay_count}, () => true);
    }
}