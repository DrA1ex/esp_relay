#pragma once

#include <cstdint>
#include "lib/bootstrap.h"
#include "lib/misc/ntp_time.h"

#include "app/metadata.h"
#include "app/type.h"
#include "misc/night_mode.h"
#include "misc/relay.h"
#include "network/api.h"

class Application {
    std::unique_ptr<Bootstrap<Config, PacketTypeEnum>> _bootstrap = nullptr;
    std::unique_ptr<NightModeManager> _night_mode_manager = nullptr;
    std::unique_ptr<NtpTime> _ntp_time = nullptr;
    std::unique_ptr<ApiWebServer> _api = nullptr;


    uint8_t _relay_count = 0;
    std::unique_ptr<std::array<std::unique_ptr<RelayManager>, ACTUAL_RELAY_COUNT>> _relays = nullptr;

    std::unique_ptr<ConfigMeta> _metadata = nullptr;
    std::map<const AbstractParameter *, PacketTypeEnum> _parameter_to_packet{};

public:
    inline Config &config() { return _bootstrap->config(); }

    void begin();
    void event_loop();

    void load();
    void update();

    void restart() { _bootstrap->restart(); }
    void set_power(bool value);

private:
    void _setup();

    void _handle_property_change(const AbstractParameter *param);
};
