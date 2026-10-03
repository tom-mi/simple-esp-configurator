#include <Configurator.h>

#include <esp_log.h>

using namespace SimpleEspConfigurator;

static Configurator config;

void setup() {
    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set("SEC", ESP_LOG_INFO);
    Serial.begin(115200);
    // Serial.setDebugOutput(true);
    Serial.println("Hello World");

    const auto foo = config.stringParameter<"foo", "Foo", "empty">();

    const auto ip = config.ipv4Parameter<"some_ip", "IP Address", "127.0.0.1", false>();

    Serial.println((std::string(">>>>") + foo).c_str());
    Serial.println(ip.toString().c_str());
    config.deviceName("hello-world");
    // config.customHomePageHtml("<p><b>This is some example text</b></p>");
    // config.customHtmlParameter("", "<h3>WiFi</h3>");
    // config.customHtmlParameter("test", "<input name='test' value='test' />", [](const std::string &value ) {
    //     Serial.println((std::string("Custom: ") + value).c_str());
    // });
    const auto wifiParameters = config.wifiParameters();
    const auto wifiNetParams = config.networkParameters();
    const ApParameters apParameters = config.apParameters();

    if (!Configurator::connectToWifiSync(wifiParameters, wifiNetParams)) {
        config.startApWithCaptivePortal(apParameters);
    }
    config.enableOtaUpdate();
    config.startWebServer();
}

void loop() {
    // nothing to do
    config.loop();
}