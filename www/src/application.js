import {ApplicationBase} from "./lib/index.js";

import {Config} from "./config.js";
import {Properties} from "./props.js";

export class Application extends ApplicationBase {
    #config = new Config(Properties);

    get config() {
        return this.#config;
    }

    get propertyConfig() {
        return Properties;
    }
}