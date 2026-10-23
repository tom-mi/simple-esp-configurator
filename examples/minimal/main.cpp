#include <Configurator.h>
#include <WiFi.h>

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

using namespace SimpleEspConfigurator;

static Configurator config;
static DNSServer dnsServer;

void setup() {
    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set("SEC", ESP_LOG_INFO);
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    Serial.print("Connected? ");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" yes");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    const auto str1 = config.stringParameter<"str1", "String", "abc", 3, 5, "", false>();

    config.deviceName("minimal");
    config.startWebServer();
}

void loop() {
    config.loop();
    dnsServer.processNextRequest();
}
