#pragma once

#include <cstdint>

#include "lib/base/application.h"
#include "lib/misc/ntp_time.h"
#include "lib/misc/storage.h"
#include "lib/network/protocol/packet_handler.h"

#include "constants.h"
#include "night_mode.h"
#include "relay.h"
#include "type.h"

class Application : public AbstractApplication<Config, AppMetadata> {
    static std::map<PacketTypeEnum, AppMetadata> PropToMetaMap;

    Storage<Config> &_config_storage;
    Timer &_timer;

    NightModeManager _night_mode_manager;

    RelayManager _relays[ACTUAL_RELAY_COUNT];

public:
    inline ConfigT &config() override { return _config_storage.get(); }
    inline SysConfig &sys_config() { return config().sys_config; }

    explicit Application(Storage<Config> &config_storage, Timer &timer, NtpTime &ntp_time);
    void begin();

    void load();
    void restart();
};

class AppPacketHandler : public PacketHandler<Application> {
public:
    explicit AppPacketHandler(Application &app) : PacketHandler<Application>(app) {}

    Response handle_packet_data(uint32_t client_id, const Packet<PacketEnumT> &packet) override;
};