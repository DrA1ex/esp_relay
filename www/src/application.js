import {ApplicationBase} from "./lib/index.js";

import {Config, PacketType} from "./config.js";
import {Properties} from "./props.js";

export class Application extends ApplicationBase {
    #config = new Config(Properties);

    get config() {
        return this.#config;
    }

    get propertyConfig() {
        return Properties;
    }

    async begin(root) {
        await super.begin(root);

        this.propertyMeta["apply_sys_config"].control.setOnClick(this.applySysConfig.bind(this));

        this.subscribe(this, this.Event.PropertyCommited, this.onPropCommited.bind(this));
    }

    async applySysConfig(sender) {
        if (sender.getAttribute("data-saving") === "true") return;

        sender.setAttribute("data-saving", true);

        try {
            await this.ws.request(PacketType.RESTART);

            let new_url
            if (location.hostname !== "localhost") {
                const url_parts = [
                    location.protocol + "//",
                    this.config.sys_config.mdns_name + ".local",
                    location.port ? ":" + location.port : "",
                    "/?" + (location.href.split("?")[1] ?? "")
                ]

                new_url = url_parts.join("");
            } else {
                new_url = location.href;
            }

            setTimeout(() => window.location = new_url, 3000);
        } catch (err) {
            console.log("Unable to send restart signal", err);
            sender.setAttribute("data-saving", false);
        }
    }

    onPropCommited(config, {key}) {
        if (key === "sys_config.relay_count") {
            this.config.refreshRelayConfig();

            for (const propKey of [
                "sys_config.relay.0.pin",
                "sys_config.relay.1.pin",
                "sys_config.relay.2.pin",
                "sys_config.relay.3.pin",
            ]) {
                this.emitEvent(this.Event.Notification, {key: propKey, value: this.config.getProperty(propKey)});
            }
        }
    }
}