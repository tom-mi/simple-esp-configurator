/**
 * Minimal firmware for OTA update testing.
 * Only includes WiFi setup and OTA update functionality.
 */
#include <Arduino.h>

#include <Configurator.h>
#include <WifiUtil.h>
#include <EthernetUtil.h>
#include <OtaUtil.h>

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
#ifndef ETH_IP
#define ETH_IP ""
#endif
#ifndef ETH_SUBNET
#define ETH_SUBNET "255.255.255.0"
#endif
#ifndef ETH_GW
#define ETH_GW "192.168.0.1"
#endif

static SimpleEspConfigurator::Configurator config;
static DNSServer dnsServer;

void setup() {
    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set("SEC", ESP_LOG_INFO);
    Serial.begin(115200);

    // Hardcoding the WiFi credentials & network setup to simplify the test setup.

    // Connect to Ethernet
    const SimpleEspConfigurator::NetworkParameters ethNetworkParameters = {
        .ip = IPAddress(ETH_IP),
        .gateway = IPAddress(ETH_GW),
        .subnet = IPAddress(ETH_SUBNET),
    };
    const bool ethConnected = SimpleEspConfigurator::connectToEthernetSync(ethNetworkParameters);
    if (ethConnected) {
        ESP_LOGI("SEC", "Connected to Ethernet");
    } else {
        ESP_LOGW("SEC", "Failed to connect to Ethernet");
    }

    // Connect to WiFi
    const SimpleEspConfigurator::WiFiParameters wifiParameters = {.ssid = WIFI_SSID, .password = WIFI_PASS};
    const SimpleEspConfigurator::NetworkParameters wifiNetworkParameters = {
        .ip = IPAddress(WIFI_IP),
        .gateway = IPAddress(WIFI_GW),
        .subnet = IPAddress(WIFI_SUBNET),
    };
    const bool wifiConnected = SimpleEspConfigurator::connectToWifiSync(wifiParameters, wifiNetworkParameters);
    if (wifiConnected) {
        ESP_LOGI("SEC", "Connected to WiFi");
    } else {
        ESP_LOGW("SEC", "Failed to connect to WiFi");
    }

    // Marker device name to identify this firmware
    config.deviceName("e2e-testing-ota");

    // Enable OTA update so firmware can be reverted
    SimpleEspConfigurator::enableOtaUpdate(config);
    config.startWebServer();
}

void loop() {
    config.loop();
}
