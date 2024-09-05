import {PacketType} from "./config.js";

/**
 * @type {PropertiesConfig}
 */
export const Properties = [{
    key: "general", section: "General", props: [
        {key: "power", title: "Power", type: "trigger", kind: "Boolean", cmd: PacketType.POWER}
    ],
}, {
    key: "relay_list", section: "Relay control", visibleIf: "relay.1", props: [
        {key: "relay.0.power", title: "Relay #1", type: "trigger", kind: "Boolean", cmd: PacketType.RELAY_POWER_0, visibleIf: "relay.0"},
        {key: "relay.1.power", title: "Relay #2", type: "trigger", kind: "Boolean", cmd: PacketType.RELAY_POWER_1, visibleIf: "relay.1"},
        {key: "relay.2.power", title: "Relay #3", type: "trigger", kind: "Boolean", cmd: PacketType.RELAY_POWER_2, visibleIf: "relay.2"},
        {key: "relay.3.power", title: "Relay #4", type: "trigger", kind: "Boolean", cmd: PacketType.RELAY_POWER_3, visibleIf: "relay.3"},

        {key: "relay.0", type: "skip"},
        {key: "relay.1", type: "skip"},
        {key: "relay.2", type: "skip"},
        {key: "relay.3", type: "skip"},
    ]
}, {
    key: "night_mode", section: "Night Mode", props: [
        {key: "night_mode.enabled", title: "Enabled", type: "trigger", kind: "Boolean", cmd: PacketType.NIGHT_MODE_ENABLED},
        {key: "night_mode.startTime", title: "Start Time", type: "time", kind: "Uint32", cmd: PacketType.NIGHT_MODE_START_TIME},
        {key: "night_mode.endTime", title: "End Time", type: "time", kind: "Uint32", cmd: PacketType.NIGHT_MODE_END_TIME},
    ]
}];