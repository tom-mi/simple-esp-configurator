#pragma once
#include <DNSServer.h>
#include "ConfiguratorCore.h"
#include "NetworkParameters.h"

namespace SimpleEspConfigurator {
    struct WiFiParameters {
        std::string ssid;
        std::string password;
    };

    struct ApParameters {
        std::string ssid;
        std::string password;
    };

    bool connectToWifiSync(const WiFiParameters &parameters, const NetworkParameters &netParams, uint16_t timeoutMs);

    bool startAp(const ApParameters &parameters);

    bool startApWithCaptivePortal(ConfiguratorCore &configurator, const ApParameters &parameters, DNSServer &dnsServer);

    void startCaptivePortal(ConfiguratorCore &configurator, DNSServer &dnsServer);
} // namespace SimpleEspConfigurator
