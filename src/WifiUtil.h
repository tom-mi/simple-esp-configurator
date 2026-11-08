#pragma once
#include <DNSServer.h>
#include "ConfiguratorCore.h"
#include "Parameters.h"
#include "Common.h"

#include "WiFi.h"

namespace SimpleEspConfigurator {
    /**
     * @brief Connect to WiFi synchronously with network parameters.
     * @param parameters The WiFi credentials (SSID and password).
     * @param netParams The network configuration (DHCP, IP, gateway, etc.).
     * @param timeoutMs The timeout in milliseconds (default: DEFAULT_TIMEOUT_MS).
     * @return True if connection was successful, false otherwise.
     */
    inline bool connectToWifiSync(const WiFiParameters &parameters, const NetworkParameters &netParams,
                                  const uint16_t timeoutMs = DEFAULT_TIMEOUT_MS) {
        if (!netParams.dhcp) {
            WiFi.config(netParams.ip, netParams.gateway, netParams.subnet, netParams.dns1, netParams.dns2);
        } else if (!netParams.hostname.empty()) {
            WiFi.setHostname(netParams.hostname.c_str());
        }
        if (parameters.ssid.empty()) {
            ESP_LOGI(SEC_LOG_TAG, "SSID empty, not connecting to WiFi.");
            return false;
        }
        ESP_LOGI(SEC_LOG_TAG, "Connecting to WiFi: ssid=%s ...", parameters.ssid.c_str());
        const unsigned long startTime = millis();
        WiFi.begin(parameters.ssid.c_str(), parameters.password.c_str());

        while (millis() - startTime < timeoutMs) {
            if (WiFi.status() == WL_CONNECTED) {
                ESP_LOGI(SEC_LOG_TAG,
                         "WiFi connected: ssid=%s ip=%s sub=%s gw=%s",
                         WiFi.SSID().c_str(), WiFi.localIP().toString().c_str(), WiFi.subnetMask().toString().c_str(),
                         WiFi.gatewayIP().toString().c_str());
                return true;
            }
            if (WiFi.status() == WL_CONNECT_FAILED) {
                ESP_LOGW(SEC_LOG_TAG, "Connecting to WiFi failed.");
                return false;
            }
        }
        ESP_LOGW(SEC_LOG_TAG, "Connecting to WiFi timed out after %d ms.", timeoutMs);
        return false;
    }

    /**
     * @brief Connect to WiFi synchronously using DHCP.
     * @param parameters The WiFi credentials (SSID and password).
     * @param timeoutMs The timeout in milliseconds (default: DEFAULT_TIMEOUT_MS).
     * @return True if connection was successful, false otherwise.
     */
    inline bool connectToWifiSync(const WiFiParameters &parameters, const uint16_t timeoutMs = DEFAULT_TIMEOUT_MS) {
        const auto netParams = NetworkParameters{.dhcp = true};
        return connectToWifiSync(parameters, netParams, timeoutMs);
    }

    /**
     * @brief Start the Access Point.
     * @param parameters The AP configuration (SSID and password).
     * @return True if AP was started successfully, false otherwise.
     */
    inline bool startAp(const ApParameters &parameters) {
        ESP_LOGI(SEC_LOG_TAG, "Starting AP: ssid=%s", parameters.ssid.c_str());
        bool result;
        if (parameters.password.empty()) {
            result = WiFi.softAP(parameters.ssid.c_str());
        } else {
            result = WiFi.softAP(parameters.ssid.c_str(), parameters.password.c_str());
        }
        if (result) { // NOLINT(*-branch-clone)
            ESP_LOGI(SEC_LOG_TAG, "AP started: ssid=%s ip=%s sub=%s gw=%s",
                     WiFi.AP.SSID().c_str(), WiFi.AP.localIP().toString().c_str(),
                     WiFi.AP.subnetMask().toString().c_str(),
                     WiFi.AP.gatewayIP().toString().c_str());
        } else {
            ESP_LOGW(SEC_LOG_TAG, "Failed to start AP.");
        }
        return result;
    }

    /**
     * @brief Start the captive portal.
     * @param configurator The ConfiguratorCore instance.
     * @param dnsServer The DNS server for the captive portal.
     */
    inline void startCaptivePortal(ConfiguratorCore &configurator, DNSServer &dnsServer) {
        ESP_LOGI(SEC_LOG_TAG, "Setting up captive portal ...");
        // See https://en.wikipedia.org/wiki/Captive_portal
        //
        // The DHCP captive portal seems to have no effect ??
        // WiFi.AP.enableDhcpCaptivePortal();

        // Needed to trigger "Sign in to a Wi-Fi network" dialogue on android
        // reference: https://github.com/tzapu/WiFiManager/issues/553#issuecomment-910222676
        configurator.server()->onNotFound([&configurator]() {
            configurator.server()->sendHeader("Location", "/");
            configurator.server()->send(307, "text/html", "");
        });

        const IPAddress ip = WiFi.softAPIP() ? WiFi.softAPIP() : WiFi.localIP(); // fallback mostly for testing
        if (dnsServer.start(53, "*", ip)) {
            ESP_LOGI(SEC_LOG_TAG, "Started captive portal DNS with ip %s", ip.toString().c_str());
        } else {
            ESP_LOGW(SEC_LOG_TAG, "Failed to start captive portal DNS with ip %s.", ip.toString().c_str());
        }
    }

    /**
     * @brief Start the Access Point with captive portal.
     * @param configurator The ConfiguratorCore instance.
     * @param parameters The AP configuration (SSID and password).
     * @param dnsServer The DNS server for the captive portal.
     * @return True if AP was started successfully, false otherwise.
     */
    inline bool startApWithCaptivePortal(ConfiguratorCore &configurator, const ApParameters &parameters,
                                         DNSServer &dnsServer) {
        if (startAp(parameters)) {
            startCaptivePortal(configurator, dnsServer);
            return true;
        }
        return false;
    }
} // namespace SimpleEspConfigurator
