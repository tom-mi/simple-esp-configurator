/**
 * Minimal firmware for OTA update testing.
 * Only includes WiFi setup and OTA update functionality.
 */
#include <Arduino.h>

#include <Configurator.h>

#include <esp_log.h>

#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif
#ifndef WIFI_PASS
#define WIFI_PASS ""
#endif
#ifndef WIFI_IP
#define WIFI_IP ""
#endif
#ifndef WIFI_SUBNET
#define WIFI_SUBNET "255.255.255.0"
#endif
#ifndef WIFI_GW
#define WIFI_GW "192.168.0.1"
#endif

static SimpleEspConfigurator::Configurator config;
static DNSServer dnsServer;

void setup() {
    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set("SEC", ESP_LOG_INFO);
    Serial.begin(115200);

    // Hardcoding the WiFi credentials & network setup to simplify the test setup.
    const SimpleEspConfigurator::WiFiParameters wifiParameters = {.ssid = WIFI_SSID, .password = WIFI_PASS};
    const SimpleEspConfigurator::NetworkParameters networkParameters = {
        .ip = IPAddress(WIFI_IP),
        .gateway = IPAddress(WIFI_GW),
        .subnet = IPAddress(WIFI_SUBNET),
    };
    if (!config.connectToWifiSync(wifiParameters, networkParameters)) {
        // No WiFi connection: start AP with captive portal for configuration
        const SimpleEspConfigurator::ApParameters apParameters = config.apParameters<"e2e_ap", "e2e_pass">();
        startApWithCaptivePortal(config, apParameters, dnsServer);
    }

    // Marker device name to identify this firmware
    config.deviceName("e2e-testing-ota");

    // Enable OTA update so firmware can be reverted
    config.enableOtaUpdate();
    config.startWebServer();
}

void loop() {
    config.loop();
}
