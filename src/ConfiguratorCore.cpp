#include "ConfiguratorCore.h"
#include <Preferences.h>
#include <base64.h>

#include <memory>

#include "Common.h"

namespace SimpleEspConfigurator {
    static auto RESTART_DELAY_MS = 200;

    static auto HTML_HTML_HEAD_CHARSET =
            "<!doctype html>\n"
            "<html>\n"
            "<head>\n"
            "<meta name='viewport' content='width=device-width, initial-scale=1, "
            "user-scalable=no' />\n"
            "<meta charset='utf-8' />\n"
            "<style>\n"
            "body{background-color:#334;position:absolute;top:0;bottom:0;left:0;right:"
            "0;margin:0;}\n"
            ".content{position:relative;min-height: 100%; max-width:400px; "
            "margin-left:auto; margin-right:auto;"
            " border-sizing:border-box;"
            " background-color:#fff; padding:10px;}\n"
            ".inner{position:relative;}\n"
            "h1{font-size:1.5em;}\n"
            "h2{font-size:1.3em;}\n"
            "input{display: block; border:1px solid "
            "#666;border-radius:0;font-size:120%;"
            " width:100%;box-sizing:border-box;}\n"
            "input[type='checkbox']{display:inline;transform:scale(1.4);width:inherit;}"
            "\n"
            ".item{margin:12px 0;}\n"
            "button{display:block;background-color:#eee;border:1px solid "
            "#666;border-radius:0;font-size:120%;"
            " width:100%;padding:5px;cursor:pointer;}\n"
            "button.primary{background-color:#ddf;border-color:#448;}\n"
            "button.danger{background-color:#fdd;border-color:#844;}\n"
            "button:hover{border-width:2px;padding:4px;}\n"
            "</style>\n";

    static auto HTML_ENDHEAD_BODY = "</head>\n"
            "<body>\n"
            "<div class='content'><div class='inner'>\n";

    static auto HTML_ENDBODY_ENDHTML = "</div></div>\n"
            "</body>\n"
            "</html>\n";

    constexpr auto ADMIN_USER_PREFIX = "admin:";


    bool ConfiguratorCore::_saveConfigFromForm() const {
        bool allValid = true;
        for (auto const f: _fields) {
            // Retrieve form value; default to empty string if missing
            String key = f->id().c_str();
            if (std::string val = _server.arg(key).c_str(); !f->validateFromForm(val)) {
                ESP_LOGW(SEC_LOG_TAG,
                         "Validation error on POST /config for field %s='%s'",
                         key.c_str(), val.c_str());
                allValid = false;
            }
        }
        if (allValid) {
            for (const auto f: _fields) {
                String key = f->id().c_str();
                std::string val = _server.arg(key).c_str();
                f->setValueFromForm(val);
            }
        }
        return allValid;
    }

    void ConfiguratorCore::addMenuItem(const std::string &path,
                                       const std::string &label,
                                       const bool admin) {
        for (const auto &itm: _menu) {
            if (itm.path == path) {
                return;
            }
        }
        _menu.push_back({.path = path, .label = label, .admin = admin});
    }

    void ConfiguratorCore::setAdminPassword(const std::string &pw) {
        _adminPw = pw;
    }

    bool ConfiguratorCore::isRequestAuthenticated() const {
        if (_adminPw.empty()) {
            return true;
        }
        const auto expected = "Basic " + base64::encode(ADMIN_USER_PREFIX + String(_adminPw.c_str()));
        return _server.header("Authorization") == expected;
    }

    void ConfiguratorCore::requestAuthentication() {
        _server.requestAuthentication();
    }

    void ConfiguratorCore::startWebServer() {
        ESP_LOGI(SEC_LOG_TAG, "Starting web server ...");
        _server.on("/", HTTP_GET, [this]() {
            _server.send(200, "text/html", _renderHomePage().c_str());
        });
        _server.on("/login", HTTP_POST, [this]() {
            if (isRequestAuthenticated()) {
                _server.send(200, "text/html", renderRedirectToHomePage("Logged in.").c_str());
            } else {
                requestAuthentication();
            }
        });
        _server.on("/config", HTTP_GET, [this]() {
            if (!isRequestAuthenticated()) {
                requestAuthentication();
                return;
            }
            _server.send(200, "text/html", _renderConfigPage(false).c_str());
        });
        _server.on("/config", HTTP_POST, [this]() {
            if (!isRequestAuthenticated()) {
                requestAuthentication();
                return;
            }
            if (_saveConfigFromForm()) {
                _server.send(200, "text/html",
                             renderRedirectToHomePage("Configuration stored. Rebooting ...").c_str());
                reboot();
            } else {
                _server.send(400, "text/html", _renderConfigPage(true).c_str());
            }
        });
        // Reboot endpoint
        _server.on("/reboot", HTTP_POST, [this]() {
            if (!isRequestAuthenticated()) {
                requestAuthentication();
                return;
            }
            _server.send(200, "text/html", renderRedirectToHomePage("Rebooting ...").c_str());
            reboot();
        });
        // Reset endpoint – clears preferences then reboots
        _server.on("/reset", HTTP_POST, [this]() {
            if (!isRequestAuthenticated()) {
                requestAuthentication();
                return;
            }
            _erasePreferences();
            _server.send(200, "text/html",
                         renderRedirectToHomePage("Configuration has been reset. Rebooting ...").c_str());
            reboot();
        });
        _server.begin();
        ESP_LOGI(SEC_LOG_TAG, "Web server started.");
    }

    void ConfiguratorCore::loop() { _server.handleClient(); }

    void ConfiguratorCore::reboot() {
        // Restart after a short delay
        delay(RESTART_DELAY_MS);
        _server.close();
        ESP.restart(); // TODO make 8266 compatible
    }

    void ConfiguratorCore::reset() {
        _erasePreferences();
        reboot();
    }

    void ConfiguratorCore::_erasePreferences() {
        ESP_LOGI(SEC_LOG_TAG, "Erasing preferences.");
        Preferences prefs;
        prefs.begin(SEC_PREF_NS);
        prefs.clear();
        prefs.end();
    }

    std::string ConfiguratorCore::_renderConfigPage(
        const bool hasValidationError) const {
        std::string body;
        if (hasValidationError) {
            body += "<div class='error'>Validation error! This should not happen. "
                    "Please check the device logs.</div>\n";
        }
        body += "<form class='configuration' action='/config' method='post'>\n";
        for (const auto f: _fields) {
            body += "<div class='item'>\n";
            body += f->generateHtml();
            body += "</div>\n";
        }
        body += "<div class='item'><button class='primary' type='submit'>Save "
                "configuration &amp; reboot</button></item>\n";
        body += "</form>\n";
        return renderPage("Configuration", body);
    }

    std::string ConfiguratorCore::renderRedirectToHomePage(
        const std::string &message) const {
        const std::string extraHead =
                "<meta http-equiv='refresh' content='2;url=/' />\n";
        const std::string body = "<p>" + message + "</p>\n" +
                                 "<p>You will be redirected to the start page.</p>\n";
        return renderPage("", body, extraHead, false);
    }

    std::string ConfiguratorCore::_renderHomePage() {
        std::string body = _customHomePageHtml;
        for (const auto &[path, label, admin]: _menu) {
            if (admin && !isRequestAuthenticated()) {
                continue;
            }
            body += "<div class='item'><form action='" + path + "'>\n";
            body += "  <button type='submit'>" + label + "</button>\n";
            body += "</form></div>\n";
        }
        if (isRequestAuthenticated()) {
            body += "<div class='item'><form action='/reboot' method='post'>\n";
            body += "  <button type='submit'>Reboot</button>\n";
            body += "</form></div>\n";
            body += "<div class='item'><form action='/reset' method='post'";
            body += " onsubmit='return confirm(\"Erase config?\");'>\n";
            body += "  <button class='danger' type='submit'>Reset configuration & "
                    "reboot</button>\n";
            body += "</form></div>\n";
        }
        if (!isRequestAuthenticated()) {
            body += "<div class='item'><form action='/login' method='post'>\n";
            body += "  <button type='submit'>Login</button>\n";
            body += "</form></div>\n";
        }
        return renderPage("", body, "", false);
    }

    std::string ConfiguratorCore::renderPage(
        const std::string &title, const std::string &body,
        const std::string &extraHead, const bool backButton) const {
        std::string out;
        out += HTML_HTML_HEAD_CHARSET;
        out += "<title>" + escapeHtml(_deviceName);
        if (!title.empty()) {
            out += " &mdash; " + escapeHtml(title);
        }
        out += "</title>\n";
        out += extraHead;
        out += HTML_ENDHEAD_BODY;
        out += "<h1>" + escapeHtml(_deviceName) + "</h1>\n";
        out += "<h2>" + escapeHtml(title) + "</h2>\n";
        out += body;
        if (backButton) {
            out += "<div class='item'><form action='/'><button "
                    "type='submit'>Back</button></form></div>\n";
        }
        out += HTML_ENDBODY_ENDHTML;
        return out;
    }

    std::string ConfiguratorCore::_generateDeviceName() {
        // TODO ESP8266
        const uint64_t chipId = ESP.getEfuseMac(); // For ESP32
        char chipMacStr[13];
        snprintf(chipMacStr, sizeof(chipMacStr), "%012llX", __bswap64(chipId));
        return std::string("ESP-") + chipMacStr;
    }
} // namespace SimpleEspConfigurator
