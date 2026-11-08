#pragma once
#include "ConfiguratorCore.h"
#include "Parameters.h"
#include "Common.h"

#include "ETH.h"

namespace SimpleEspConfigurator {
    inline bool connectToEthernetSync(const NetworkParameters &netParams, const uint16_t timeoutMs) {
        if (!netParams.dhcp) {
            ETH.config(netParams.ip, netParams.gateway, netParams.subnet, netParams.dns1, netParams.dns2);
        } else if (!netParams.hostname.empty()) {
            ETH.setHostname(netParams.hostname.c_str());
        }
        ESP_LOGI(SEC_LOG_TAG, "Connecting to Ethernet ...");
        const unsigned long startTime = millis();
        ETH.begin();

        while (millis() - startTime < timeoutMs) {
            if (ETH.linkUp()) {
                ESP_LOGI(SEC_LOG_TAG,
                         "Ethernet connected: ip=%s sub=%s gw=%s",
                         ETH.localIP().toString().c_str(), ETH.subnetMask().toString().c_str(),
                         ETH.gatewayIP().toString().c_str());
                return true;
            }
        }
        ESP_LOGW(SEC_LOG_TAG, "Connecting to Ethernet timed out after %d ms.", timeoutMs);
        return false;
    }
} // namespace SimpleEspConfigurator
