#include <Configurator.h>
#include <WifiUtil.h>

#include <esp_log.h>

static SimpleEspConfigurator::Configurator config;

void setup() {
    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set("SEC", ESP_LOG_INFO);
    Serial.begin(115200);

    const auto wifiParameters = config.wifiParameters();
    const auto apParameters = config.apParameters();

    if (!SimpleEspConfigurator::connectToWifiSync(wifiParameters)) {
        SimpleEspConfigurator::startAp(apParameters);
    }

    config.startWebServer();
}

void loop() {
    config.loop();
}
