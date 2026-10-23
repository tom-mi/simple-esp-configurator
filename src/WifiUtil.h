#pragma once
#include <DNSServer.h>
#include "ConfiguratorCore.h"

namespace SimpleEspConfigurator {
    class Configurator;

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
    };

    constexpr auto DEFAULT_TIMEOUT_MS = 10000;

    bool connectToWifiSync(const WiFiParameters &parameters, uint16_t timeoutMs = DEFAULT_TIMEOUT_MS);

    bool connectToWifiSync(const WiFiParameters &parameters, const NetworkParameters &netParams,
                           uint16_t timeoutMs = DEFAULT_TIMEOUT_MS);

    bool startAp(const ApParameters &parameters);

    bool startApWithCaptivePortal(Configurator &configurator, const ApParameters &parameters, DNSServer &dnsServer);

    void startCaptivePortal(Configurator &configurator, DNSServer &dnsServer);
} // namespace SimpleEspConfigurator
