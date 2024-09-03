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

public:
    explicit Application(Storage<Config> &config_storage);

    inline ConfigT &config() override { return _config_storage.get(); }

    void begin();

    void load();
};

typedef PacketHandler<Application> AppPacketHandler;