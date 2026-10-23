#include <Configurator.h>

#include <esp_log.h>

using namespace SimpleEspConfigurator;

static Configurator config;

void setup() {
    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set("SEC", ESP_LOG_INFO);
    Serial.begin(115200);

    const auto wifiParameters = config.wifiParameters();
    const ApParameters apParameters = config.apParameters();

    if (!connectToWifiSync(wifiParameters)) {
        Configurator::startAp(apParameters);
    }

    config.startWebServer();
}

void loop() {
    config.loop();
}
