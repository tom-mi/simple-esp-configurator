#pragma once
#include <string>
#include <IPAddress.h>

namespace SimpleEspConfigurator {
    struct NetworkParameters {
        bool dhcp;
        IPAddress ip;
        IPAddress gateway;
        IPAddress subnet;
        IPAddress dns1;
        IPAddress dns2;
        std::string hostname;
    };
} // namespace SimpleEspConfigurator
