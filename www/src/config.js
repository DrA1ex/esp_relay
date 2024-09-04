import {AppConfigBase} from "./lib/index.js";

export const DEFAULT_ADDRESS = "esp_relay.local";


export const PacketType = {
    POWER: 0x00,
    NIGHT_MODE_ENABLED: 0x40,
    NIGHT_MODE_START_TIME: 0x41,
    NIGHT_MODE_END_TIME: 0x42,

    GET_CONFIG: 0x80
}

export class Config extends AppConfigBase {
    power;
    night_mode;


    get cmd() {
        return PacketType.GET_CONFIG;
    }

    parse(parser) {
        this.power = parser.readBoolean();
        this.night_mode = {
            enabled: parser.readBoolean(),
            startTime: parser.readUint32(),
            endTime: parser.readUint32()
        };
    }
}