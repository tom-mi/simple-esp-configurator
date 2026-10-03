/**
 * Special firmware to be used for the automated e2e tests (see folder e2e test).
 * It tries to cover everything that can conveniently be tested automatically, without needing control over the wifi
 * on the machine running the tests.
 */
#include <Arduino.h>

#include <Configurator.h>

#include <esp_log.h>

using namespace SimpleEspConfigurator;

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

static Configurator config;

void setup() {
    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set("SEC", ESP_LOG_INFO);
    Serial.begin(115200);

    // Hardcoding the WiFi credentials & network setup to simplify the test setup.
    // We still test storing and retrieving credentials via the UI, just not passing those dynamically.
    const WiFiParameters wifiParameters = {.ssid = WIFI_SSID, .password = WIFI_PASS};
    const NetworkParameters networkParameters = {
        .ip = IPAddress(WIFI_IP),
        .gateway = IPAddress(WIFI_GW),
        .subnet = IPAddress(WIFI_SUBNET),
    };
    config.connectToWifiSync(wifiParameters, networkParameters);

    // fields to be tested
    const auto str1 = config.stringParameter<"str1", "String", "abc", 3, 5, "", false>();
    const auto str2 = config.stringParameter<"str2", "Regex", "Abc", 0, INT_MAX, "[A-Z][a-z]+">();
    const auto str3 = config.stringParameter<"str3", "String3", "", -1, -1, "", false>();
    const auto ip1 = config.ipv4Parameter<"ip1", "IP1", "127.0.0.1", false>();
    const auto ip2 = config.ipv4Parameter<"ip2", "IP2", "127.0.0.2">();
    const auto bool1 = config.boolParameter<"bool1", "Boolean1", true>();
    const auto bool2 = config.boolParameter<"bool2", "Boolean2", false>();
    const auto double1 = config.doubleParameter<"double1", "Double1", 42.0, -1000000.0, 1000000.0>();
    const auto double2 = config.doubleParameter<"double2", "Double2", 3.14, 1.0, 10.0>();
    const auto double3 = config.doubleParameter<"double3", "Double3", 2.1, 0.1, 5.0, 0.5>();
    const auto long1 = config.longParameter<"long1", "Long1", 42L, -1000000000L, 1000000000L>();
    const auto long2 = config.longParameter<"long2", "Long2", 1000L, 100L, 10000L>();
    const auto ulong1 = config.ulongParameter<"ulong1", "ULong1", 42UL, 0UL, 1000000000UL>();
    const auto ulong2 = config.ulongParameter<"ulong2", "ULong2", 1000UL, 100UL, 10000UL>();
    const auto p1 = config.passwordParameter<"p1", "Password1", "s3cr3t", 4, 8>();
    const auto p2 = config.passwordParameter<"p2", "Password2", "", -1, -1, false>();
    const auto ap1 = config.apParameters<"my_ap_ssid", "my_ap_pass">();
    const auto wifi1 = config.wifiParameters<"my_ssid", "my_pass1", "wifi1_", "MyWiFi">();
    const auto net1 = config.networkParameters<
        false, "net1_", "MyNetwork",
        "192.168.10.2", "192.168.10.1", "255.255.0.0",
        "1.2.3.4", "5.6.7.8"
    >();
    config.customHtmlParameter<
        "custom1",
        "<span>Some visible content here</span>"
    >();
    std::string fieldBody = "<table>";
    fieldBody += "<tr><td>str1</td><td>" + str1 + "</td></tr>";
    fieldBody += "<tr><td>str2</td><td>" + str2 + "</td></tr>";
    fieldBody += "<tr><td>str3</td><td>" + str3 + "</td></tr>";
    fieldBody += "<tr><td>ip1</td><td>" + std::string(ip1.toString().c_str()) + "</td></tr>";
    fieldBody += "<tr><td>ip2</td><td>" + std::string(ip2.toString().c_str()) + "</td></tr>";
    fieldBody += "<tr><td>bool1</td><td>" + std::to_string(bool1) + "</td></tr>";
    fieldBody += "<tr><td>bool2</td><td>" + std::to_string(bool2) + "</td></tr>";
    fieldBody += "<tr><td>double1</td><td>" + std::to_string(double1) + "</td></tr>";
    fieldBody += "<tr><td>double2</td><td>" + std::to_string(double2) + "</td></tr>";
    fieldBody += "<tr><td>double3</td><td>" + std::to_string(double3) + "</td></tr>";
    fieldBody += "<tr><td>long1</td><td>" + std::to_string(long1) + "</td></tr>";
    fieldBody += "<tr><td>long2</td><td>" + std::to_string(long2) + "</td></tr>";
    fieldBody += "<tr><td>ulong1</td><td>" + std::to_string(ulong1) + "</td></tr>";
    fieldBody += "<tr><td>ulong2</td><td>" + std::to_string(ulong2) + "</td></tr>";
    fieldBody += "<tr><td>p1</td><td>" + p1 + "</td></tr>";
    fieldBody += "<tr><td>p2</td><td>" + p2 + "</td></tr>";
    fieldBody += "<tr><td>ap1_ssid</td><td>" + ap1.ssid + "</td></tr>";
    fieldBody += "<tr><td>ap1_pass</td><td>" + ap1.password + "</td></tr>";
    fieldBody += "<tr><td>wifi1_ssid</td><td>" + wifi1.ssid + "</td></tr>";
    fieldBody += "<tr><td>wifi1_pass</td><td>" + wifi1.password + "</td></tr>";
    fieldBody += "<tr><td>net1_dhcp</td><td>" + std::to_string(net1.dhcp) + "</td></tr>";
    fieldBody += "<tr><td>net1_ip</td><td>" + std::string(net1.ip.toString().c_str()) + "</td></tr>";
    fieldBody += "<tr><td>net1_gw</td><td>" + std::string(net1.gateway.toString().c_str()) + "</td></tr>";
    fieldBody += "<tr><td>net1_sub</td><td>" + std::string(net1.subnet.toString().c_str()) + "</td></tr>";
    fieldBody += "<tr><td>net1_dns1</td><td>" + std::string(net1.dns1.toString().c_str()) + "</td></tr>";
    fieldBody += "<tr><td>net1_dns2</td><td>" + std::string(net1.dns2.toString().c_str()) + "</td></tr>";
    fieldBody += "</table>";
    const auto fieldResponse = config.renderPage("Fields", fieldBody);


    // features to be tested
    config.deviceName("e2e-testing");
    config.customHomePageHtml("<p><b>This is custom page html</b></p>");

    const auto server = config.server();
    config.server()->on("/fields", [server, fieldResponse] {
        server->send(200, "text/html", fieldResponse.c_str());
    });
    config.enableOtaUpdate();
    config.startWebServer();
}

void loop() {
    config.loop();
}
