#include "Configurator.h"
#include "WiFi.h"
#include <Update.h>


namespace SimpleEspConfigurator {

bool Configurator::connectToWifiSync(const WiFiParameters &parameters, const NetworkParameters &netParams,
                                              const uint16_t timeoutMs) {
    if (!netParams.dhcp) {
        WiFi.config(netParams.ip, netParams.gateway, netParams.subnet, netParams.dns1, netParams.dns2);
    }
    return connectToWifiSync(parameters, timeoutMs);
}

bool Configurator::connectToWifiSync(const WiFiParameters &parameters,
                                              const uint16_t timeoutMs) {
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

bool Configurator::startAp(const ApParameters &parameters) {
    ESP_LOGI(SEC_LOG_TAG, "Starting AP: ssid=%s", parameters.ssid.c_str());
    bool result;
    if (parameters.password.empty()) {
        result = WiFi.softAP(parameters.ssid.c_str());
    } else {
        result = WiFi.softAP(parameters.ssid.c_str(), parameters.password.c_str());
    }
    if (result) { // NOLINT(*-branch-clone)
        ESP_LOGI(SEC_LOG_TAG, "AP started: ssid=%s ip=%s sub=%s gw=%s",
                 WiFi.AP.SSID().c_str(), WiFi.AP.localIP().toString().c_str(), WiFi.AP.subnetMask().toString().c_str(),
                 WiFi.AP.gatewayIP().toString().c_str());
    } else {
        ESP_LOGW(SEC_LOG_TAG, "Failed to start AP.");
    }
    return result;
}

bool Configurator::startApWithCaptivePortal(const ApParameters &parameters, DNSServer &dnsServer) {
    if (startAp(parameters)) {
        startCaptivePortal(dnsServer);
        return true;
    }
    return false;
}

void Configurator::startCaptivePortal(DNSServer &dnsServer) {
    ESP_LOGI(SEC_LOG_TAG, "Setting up captive portal ...");
    // See https://en.wikipedia.org/wiki/Captive_portal
    //
    // The DHCP captive portal seems to have no effect ??
    // WiFi.AP.enableDhcpCaptivePortal();

    // Needed to trigger "Sign in to a Wi-Fi network" dialogue on android
    // reference: https://github.com/tzapu/WiFiManager/issues/553#issuecomment-910222676
    _server.onNotFound([this]() {
        _server.sendHeader("Location", "/");
        _server.send(307, "text/html", "");
    });

    const IPAddress ip = WiFi.softAPIP() ? WiFi.softAPIP() : WiFi.localIP(); // fallback mostly for testing
    if (dnsServer.start(53, "*", ip)) {
        ESP_LOGI(SEC_LOG_TAG, "Started captive portal DNS with ip %s", ip.toString().c_str());
    } else {
        ESP_LOGW(SEC_LOG_TAG, "Failed to start captive portal DNS with ip %s.", ip.toString().c_str());
    }
}

// Handle the upload data chunk by chunk. No response is sent; this
// function is used as the second callback for server.on().
void Configurator::_handleOtaUpload() {
    size_t fsize = UPDATE_SIZE_UNKNOWN;
    if (_server.hasArg("size")) {
        fsize = _server.arg("size").toInt();
    }
    HTTPUpload &upload = _server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        if (!Update.begin(fsize)) {
            ESP_LOGE(SEC_LOG_TAG, "OTA init failed: %s", Update.errorString());
            return;
        }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            ESP_LOGE(SEC_LOG_TAG, "OTA write error: %s", Update.errorString());
        }
    }
}

// Finalize OTA, send response and reboot if successful. This function is used as the
// first callback in server.on() for POST.
void Configurator::_handleOtaFinalize() {
    if (Update.end(true)) {
        ESP_LOGI(SEC_LOG_TAG, "OTA update succeeded.");
        // Redirect to home after a short delay
        _server.send(200, "text/html", _renderRebootPage("OTA update succeeded.").c_str());
        delay(500);
        ESP.restart();
    } else {
        ESP_LOGE(SEC_LOG_TAG, "OTA update failed: %s", Update.errorString());
        _server.send(500, "text/plain", Update.errorString());
    }
}

std::string Configurator::_renderOtaPage() const {
    const std::string body =
            "<form method='post' enctype='multipart/form-data' action='/ota'"
            " onsubmit='return confirm(\"Overwrite firmware?\");'>\n"
            "<div class='item'><input type='file' name='file' required accept='.bin' /></item>\n"
            "<div class='item'><button class='danger' type='submit'>Upload</button></div>\n"
            "</form>\n";
    return renderPage("OTA update", body, "", true);
}

void Configurator::loop() {
    ConfiguratorCore::loop();
}

void Configurator::enableOtaUpdate() {
    _server.on("/update", HTTP_GET, [this]() {
        _server.send(200, "text/html", this->_renderOtaPage().c_str());
    });
    // Separate callbacks: first sends response after upload, second processes upload data
    _server.on("/update", HTTP_POST, [this]() {
                       this->_handleOtaFinalize();
                   }, [this]() {
                       this->_handleOtaUpload();
                   });
    addMenuItem("/update", "OTA update");
}

} // namespace SimpleEspConfigurator
