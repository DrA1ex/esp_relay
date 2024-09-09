import {PacketType} from "./config.js";

/**
 * @type {PropertiesConfig}
 */
export const Properties = [{
    key: "general", section: "General", props: [
        {key: "power", title: "Power", type: "trigger", kind: "Boolean", cmd: PacketType.POWER}
    ],
}, {
    key: "relay_list", section: "Relay control", visibleIf: "_relay_cfg.0", props: [
        {key: "relay.0.power", title: "Relay #1", type: "trigger", kind: "Boolean", cmd: PacketType.RELAY_POWER_0, visibleIf: "_relay_cfg.0"},
        {key: "relay.1.power", title: "Relay #2", type: "trigger", kind: "Boolean", cmd: PacketType.RELAY_POWER_1, visibleIf: "_relay_cfg.1"},
        {key: "relay.2.power", title: "Relay #3", type: "trigger", kind: "Boolean", cmd: PacketType.RELAY_POWER_2, visibleIf: "_relay_cfg.2"},
        {key: "relay.3.power", title: "Relay #4", type: "trigger", kind: "Boolean", cmd: PacketType.RELAY_POWER_3, visibleIf: "_relay_cfg.3"},

        {key: "_relay_cfg.0", type: "skip"},
        {key: "_relay_cfg.1", type: "skip"},
        {key: "_relay_cfg.2", type: "skip"},
        {key: "_relay_cfg.3", type: "skip"},
    ]
}, {
    key: "night_mode", section: "Night Mode", collapse: "true", props: [
        {key: "night_mode.enabled", title: "Enabled", type: "trigger", kind: "Boolean", cmd: PacketType.NIGHT_MODE_ENABLED},
        {key: "night_mode.startTime", title: "Start Time", type: "time", kind: "Uint32", cmd: PacketType.NIGHT_MODE_START_TIME},
        {key: "night_mode.endTime", title: "End Time", type: "time", kind: "Uint32", cmd: PacketType.NIGHT_MODE_END_TIME},
    ]
}, {
    key: "settings", section: "Settings", collapse: "true", props: [
        {key: "sys_config.mdns_name", title: "mDNS name", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MDNS_NAME, maxLength: 32},

        {type: "title", label: "WiFi"},
        {key: "sys_config.wifi_mode", title: "Mode", type: "select", kind: "Uint8", cmd: PacketType.SYS_CONFIG_WIFI_MODE, list: "wifi_mode"},
        {key: "sys_config.wifi_ssid", title: "SSID", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_WIFI_SSID, maxLength: 32},
        {key: "sys_config.wifi_password", title: "Password", type: "password", kind: "FixedString", cmd: PacketType.SYS_CONFIG_WIFI_PASSWORD, maxLength: 32},

        {type: "title", label: "WiFi Extra"},
        {key: "sys_config.wifi_connection_check_interval", title: "Connection Check Interval", type: "int", kind: "Uint32", cmd: PacketType.SYS_CONFIG_WIFI_CONNECTION_CHECK_INTERVAL},
        {key: "sys_config.wifi_max_connection_attempt_interval", title: "Max Connection Attempt Interval", type: "int", kind: "Uint32", cmd: PacketType.SYS_CONFIG_WIFI_MAX_CONNECTION_ATTEMPT_INTERVAL},

        {type: "title", label: "Relay PIN"},

        {key: "sys_config.relay_count", title: "Count", type: "int", kind: "Uint8", cmd: PacketType.SYS_CONFIG_RELAY_COUNT, min: 1, limit: 4},
        {key: "sys_config.relay.0.pin", title: "Relay #1", type: "int", kind: "Uint8", cmd: PacketType.SYS_CONFIG_RELAY_PIN_0, visibleIf: "_relay_cfg.0"},
        {key: "sys_config.relay.1.pin", title: "Relay #2", type: "int", kind: "Uint8", cmd: PacketType.SYS_CONFIG_RELAY_PIN_1, visibleIf: "_relay_cfg.1"},
        {key: "sys_config.relay.2.pin", title: "Relay #3", type: "int", kind: "Uint8", cmd: PacketType.SYS_CONFIG_RELAY_PIN_2, visibleIf: "_relay_cfg.2"},
        {key: "sys_config.relay.3.pin", title: "Relay #4", type: "int", kind: "Uint8", cmd: PacketType.SYS_CONFIG_RELAY_PIN_3, visibleIf: "_relay_cfg.3"},

        {type: "title", label: "Relay Extra"},
        {key: "sys_config.relay_high_state", title: "High State", type: "trigger", kind: "Boolean", cmd: PacketType.SYS_CONFIG_RELAY_HIGH_STATE},
        {key: "sys_config.relay_initial_state", title: "Initial State", type: "trigger", kind: "Boolean", cmd: PacketType.SYS_CONFIG_RELAY_INITIAL_STATE},
        {key: "sys_config.relay_force_initial_state", title: "Force Initial State", type: "trigger", kind: "Boolean", cmd: PacketType.SYS_CONFIG_RELAY_FORCE_INITIAL_STATE},
        {key: "sys_config.relay_switch_interval", title: "Switch Interval", type: "int", kind: "Uint32", cmd: PacketType.SYS_CONFIG_RELAY_SWITCH_INTERVAL,},

        {type: "title", label: "Time"},
        {key: "sys_config.timezone", title: "Timezone", type: "int", kind: "Float32", cmd: PacketType.SYS_CONFIG_TIMEZONE},

        {type: "title", label: "MQTT"},
        {key: "sys_config.mqtt", title: "Enabled", type: "trigger", kind: "Boolean", cmd: PacketType.SYS_CONFIG_MQTT_ENABLED},
        {key: "sys_config.mqtt_host", title: "Host", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MQTT_HOST, maxLength: 32},
        {key: "sys_config.mqtt_port", title: "Port", type: "int", kind: "Uint16",  cmd: PacketType.SYS_CONFIG_MQTT_PORT, min: 1, limit: 65535},
        {key: "sys_config.mqtt_user", title: "User", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MQTT_USER, maxLength: 32},
        {key: "sys_config.mqtt_password", title: "Password", type: "password", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MQTT_PASSWORD, maxLength: 32},

        {type: "title", label: "Actions", extra: {m_top: true}},
        {key: "apply_sys_config",type: "button", label: "Apply Settings"}
    ]
}];