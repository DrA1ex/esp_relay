#pragma once

#include <cstdint>

#include "lib/base/application.h"
#include "lib/misc/ntp_time.h"
#include "lib/misc/storage.h"
#include "lib/network/protocol/packet_handler.h"

#include "constants.h"
#include "night_mode.h"
#include "type.h"

class Application : public AbstractApplication<Config, AppMetadata> {
    static std::map<PacketTypeEnum, AppMetadata> PropToMetaMap;

    Storage<Config> &_config_storage;
    Timer &_timer;
    NtpTime &ntp_time;

    NightModeManager _night_mode_manager;

    bool _relay_state = RELAY_INITIAL_STATE;
    unsigned long _last_relay_update = 0;
    unsigned long _relay_update_timer = -1ul;

public:
    inline ConfigT &config() override { return _config_storage.get(); }

    explicit Application(Storage<Config> &config_storage, Timer &timer, NtpTime &ntp_time);
    void begin();

    void load();

    void update_relay_state(bool state);

private:
    void _relay_timer_handler(bool state);
};

class AppPacketHandler : public PacketHandler<Application> {
public:
    explicit AppPacketHandler(Application &app) : PacketHandler<Application>(app) {}

    Response handle_packet_data(uint32_t client_id, const Packet<PacketEnumT> &packet) override;
};