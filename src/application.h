#pragma once

#include <cstdint>

#include "lib/base/application.h"
#include "lib/misc/storage.h"
#include "lib/network/protocol/packet_handler.h"

#include "constants.h"
#include "type.h"

class Application : public AbstractApplication<Config, AppMetadata> {
    static std::map<PropertyEnum, AppMetadata> PropToMetaMap;

    Storage<Config> &_config_storage;
    Timer &_timer;

    bool _relay_state = RELAY_INITIAL_STATE;
    unsigned long _last_relay_update = 0;
    unsigned long _relay_update_timer = -1ul;

public:
    inline ConfigT &config() override { return _config_storage.get(); }

    explicit Application(Storage<Config> &config_storage, Timer &timer);
    void begin();

    void load();
    void update_relay_state(bool state);
};

typedef PacketHandler<Application> AppPacketHandler;