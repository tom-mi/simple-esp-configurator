#include "OtaUtil.h"
#include "Configurator.h"
#include <Update.h>

namespace SimpleEspConfigurator {
    // Handle the upload data chunk by chunk. No response is sent; this
    // function is used as the second callback for server.on().
    static void handleOtaUpload(ConfiguratorCore &configurator) {
        auto *server = configurator.server();
        size_t fsize = UPDATE_SIZE_UNKNOWN;
        if (server->hasArg("size")) {
            fsize = server->arg("size").toInt();
        }
        HTTPUpload &upload = server->upload();
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
    static void handleOtaFinalize(ConfiguratorCore &configurator) {
        auto *server = configurator.server();
        if (Update.end(true)) {
            ESP_LOGI(SEC_LOG_TAG, "OTA update succeeded.");
            // Redirect to home after a short delay
            server->send(200, "text/html",
                         configurator.renderRedirectToHomePage("OTA update succeeded. Rebooting ...").c_str());
            delay(500);
            ESP.restart();
        } else {
            ESP_LOGE(SEC_LOG_TAG, "OTA update failed: %s", Update.errorString());
            server->send(500, "text/plain", Update.errorString());
        }
    }

    static std::string renderOtaPage(const ConfiguratorCore &configurator) {
        const std::string body =
                "<form method='post' enctype='multipart/form-data' action='/update'"
                " onsubmit='return confirm(\"Overwrite firmware?\");'>\n"
                "<div class='item'><input type='file' name='file' required accept='.bin' /></item>\n"
                "<div class='item'><button class='danger' type='submit'>Upload</button></div>\n"
                "</form>\n";
        return configurator.renderPage("OTA update", body, "", true);
    }

    void enableOtaUpdate(ConfiguratorCore &configurator) {
        auto *server = configurator.server();
        server->on("/update", HTTP_GET, [&configurator, server]() {
            if (!configurator.isRequestAuthenticated()) {
                server->sendHeader("WWW-Authenticate", "Basic realm=\"OTA Update\"");
                server->send(401);
                return;
            }
            server->send(200, "text/html", renderOtaPage(configurator).c_str());
        });
        // Separate callbacks: first sends response after upload, second processes upload data
        server->on("/update", HTTP_POST, [&configurator]() {
                       if (!configurator.isRequestAuthenticated()) {
                           configurator.requestAuthentication();
                           return;
                       }
                       handleOtaFinalize(configurator);
                   }, [&configurator]() {
                       if (!configurator.isRequestAuthenticated()) {
                           return;
                       }
                       handleOtaUpload(configurator);
                   });
        configurator.addMenuItem("/update", "OTA update");
    }
} // namespace SimpleEspConfigurator
