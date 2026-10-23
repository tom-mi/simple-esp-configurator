#include <Configurator.h>

#include <esp_log.h>

static SimpleEspConfigurator::Configurator config;
static DNSServer dnsServer;

void setup() {
    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set("SEC", ESP_LOG_INFO);
    Serial.begin(115200);

    // === register user-defined parameters ===
    // the values are available after calling the parameter factory method, which
    // * adds a field to the configuration page allowing to change the value
    // * reads the stored value from the config or returns the default
    const auto foo = config.stringParameter<"foo", "Foo", "empty">();
    const auto secret = config.passwordParameter<"secret", "Secret", "s3cr3t", 4, 8>();
    const auto ip = config.ipv4Parameter<"some_ip", "IP Address", "127.0.0.1", false>();
    const auto number = config.longParameter<"number", "Number", 42, 0, 100>();
    const auto ratio = config.doubleParameter<"ratio", "Ratio", 3.14, 0.0, 10.0>();
    const auto enabled = config.boolParameter<"enabled", "Enabled", true>();

    Serial.println(secret.c_str());
    Serial.println(ip.toString().c_str());
    Serial.println(number);
    Serial.println(ratio);
    Serial.println(enabled);

    // Sets the device name shown on the config page.
    config.deviceName("hello-world");

    // Inserts raw HTML into the config page.
    config.customHtmlParameter<"custom", "<h3>WiFi</h3>">();

    // For Wi-Fi, there are higher-level structs containing a couple of parameters. You can use those in whatever way
    // you want, the canonical way however is to connect to the configured Wi-Fi and/or start an access point for
    // initial setup (see below).
    const auto wifiParameters = config.wifiParameters();
    const auto wifiNetParams = config.networkParameters();
    const auto apParameters = config.apParameters();

    // Blocks until the stored Wi-Fi network is joined (or the timeout expires).
    if (!config.connectToWifiSync(wifiParameters, wifiNetParams)) {
        // No Wi-Fi connection: start an access point with a captive portal for configuration.
        config.startApWithCaptivePortal(apParameters, dnsServer);
    }

    // Allows updating the firmware over the air from a OTA subpage in the web portal.
    config.enableOtaUpdate();

    // Starts the web server that serves the config page.
    config.startWebServer();
}

void loop() {
    // Processes web server and OTA requests; must be called from the main loop.
    config.loop();
}
