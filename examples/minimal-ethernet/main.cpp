#include <Configurator.h>
#include <EthernetUtil.h>

#include <esp_log.h>

static SimpleEspConfigurator::Configurator config;

void setup() {
    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set("SEC", ESP_LOG_INFO);
    Serial.begin(115200);

    const auto netParams = config.networkParameters<"eth_", "Ethernet">(true);

    if (!SimpleEspConfigurator::connectToEthernetSync(netParams)) {
        // Handle connection failure (e.g., start AP for configuration)
        Serial.println("Ethernet connection failed");
    }

    config.startWebServer();
}

void loop() {
    config.loop();
}
