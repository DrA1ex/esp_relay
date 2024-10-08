# esp-relay
Smart Relay Control for ESP32/ESP8266

<a href="https://espressif.github.io/esp-launchpad/?flashConfigURL=https://dra1ex.github.io/esp-launchpad/config.toml">
    <img alt="Try it with ESP Launchpad" src="https://espressif.github.io/esp-launchpad/assets/try_with_launchpad.png" width="180">
</a>

## UI

![UI](https://github.com/user-attachments/assets/79f10d8c-4b00-4d3c-b53a-f4702755c636)


## Features
- Web/Mobile Application (PWA)
- Integration with any Smart Home Assistant (such as Alise) via MQTT broker
- Web Hooks
- MQTT Protocol

## Connection

<img width="500" alt="image" src="https://github.com/user-attachments/assets/03cba235-0981-460f-9fcf-69f6f5043c81">
<img width="500" alt="image" src="https://github.com/user-attachments/assets/82925a6e-1be5-4dda-b36d-2f3ed92ee5a9">


## Installation

### Web Installer

1. Open the [ESP Launchpad](https://espressif.github.io/esp-launchpad/?flashConfigURL=https://dra1ex.github.io/esp-launchpad/config.toml).
2. Select **esp-relay** and your desired platform.

### Manual Build / OTA

1. Install [PlatformIO](https://platformio.org/install).
2. (Optional) Modify the `credentials.h` file and, if necessary, customize `constants.h`. You can change these parameters later through the Web UI.
3. Upload the filesystem and firmware.

**Note:** This repository contains a submodule. Please use the `--recursive` option when cloning.

```sh
git clone --recursive https://github.com/DrA1ex/esp_relay.git
cd esp_relay

# Make script executable
chmod +x ./upload_fs.sh

# Specify the platform: esp32-c3 or esp8266
PLATFORM=esp32-c3

# Set the environment: debug, release, or ota
ENV=release

# For OTA: set your ESP's address 
ADDRESS=esp_relay.local

# Additional envs if OTA enabled
if [ "$ENV" = "ota" ]; then OTA=1 else OTA=0 ADDRESS= fi

pio run -t upload -e $PLATFORM-$ENV --upload-port "$ADDRESS"
./upload_fs.sh --upload-port "$ADDRESS"
```

## MQTT Protocol

| Topic In *       			| Topic Out *          			| Type        | Values		         | Comments                              |
|---------------------------|-------------------------------|-------------|----------------------|---------------------------------------|
| `MQTT_TOPIC_POWER`		| `MQTT_OUT_TOPIC_POWER` 		| `uint8_t`   | 0..1      	     	 | Power state: ON (1) / OFF (0)         |
| `MQTT_TOPIC_NIGHT_MODE`	| `MQTT_OUT_TOPIC_NIGHT_MODE` 	| `uint8_t`   | 0..1          		 | Night mode state: ON (1) / OFF (0)    |

\* Actual topic values decalred in `constants.h`

## Misc

### Configuring a Secure WebSocket Proxy with Nginx

If you're hosting a Web UI that uses SSL, you'll need to set up a Secure WebSocket (`wss://...`) server instead of the non-secure `ws://` provided by your ESP. Browsers require secure socket connections for WebSocket functionality, so this configuration is essential.

To achieve this, you can use Nginx as a proxy to create an SSL-enabled WebSocket connection.

#### Step 0: Install Nginx

```sh
apt install nginx
```

#### Step 1: Create the Nginx Configuration

Create a file at `/etc/nginx/conf.d/ws.locations` and add the following content:

```nginx
location /w_esp_relay/ws {
    proxy_pass http://<YOUR-ESP-IP-HERE_1>/ws; # Replace with your actual service address
    proxy_http_version 1.1;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection keep-alive;
    proxy_set_header Host $host;
}

# You can create proxy for multiple hosts
location /w_esp_relay_2/ws {
    proxy_pass http://<YOUR-ESP-IP-HERE_2>/ws; # Replace with your actual service address
    proxy_http_version 1.1;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection keep-alive;
    proxy_set_header Host $host;
}
```

#### Step 2: Reload the Nginx Configuration

After saving the configuration file, reload Nginx to apply the changes:

```sh
nginx -s reload
```

**Note**
Make sure to replace `<YOUR-ESP-IP-HERE_1>` and `<YOUR-ESP-IP-HERE_2>` with the actual IP addresses of your ESP devices.

#### Step 3: Check result

Open WebUi in browser https://dra1ex.github.io/esp_relay/?host=ADDRESS/w_esp_relay
