import {PacketType} from "./config.js";

/**
 * @type {PropertiesConfig}
 */
export const Properties = [{
    key: "general", section: "General", props: [
        {key: "power", title: "Power", type: "trigger", kind: "Boolean", cmd: PacketType.POWER}
    ],
}, {
    key: "night_mode", section: "Night Mode", props: [
        {key: "night_mode.enabled", title: "Enabled", type: "trigger", kind: "Boolean", cmd: PacketType.NIGHT_MODE_ENABLED},
        {key: "night_mode.startTime", title: "Start Time", type: "time", kind: "Uint32", cmd: PacketType.NIGHT_MODE_START_TIME},
        {key: "night_mode.endTime", title: "End Time", type: "time", kind: "Uint32", cmd: PacketType.NIGHT_MODE_END_TIME},
    ]
}];