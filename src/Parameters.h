#pragma once
#include <string>
#include <IPAddress.h>

namespace SimpleEspConfigurator {
    struct WiFiParameters {
        std::string ssid;
        std::string password;
    };

    struct ApParameters {
        std::string ssid;
        std::string password;
    };

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
