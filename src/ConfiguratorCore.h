#pragma once
#include <memory>
#include <string>
#include <vector>
// TODO make this Esp8266 compatible
#include <WebServer.h>

#include "fields/ConfiguratorField.h"

namespace SimpleEspConfigurator {
    class ConfiguratorCore {
    public:
        ConfiguratorCore() { addMenuItem("/config", "Configuration"); }

        virtual ~ConfiguratorCore() = default;


        void deviceName(const std::string &name) { _deviceName = name; }
        /**
         * @brief Set custom HTML to prepend to the rendered home page body.
         */
        void customHomePageHtml(const std::string &html) {
            _customHomePageHtml = html;
        }

        WebServer *server() { return &_server; };

        void startWebServer();

        virtual void loop();

        /**
         * @brief Reboot the ESP device.
         */
        void reboot();

        /**
         * @brief Reset stored configuration and reboot.
         */
        void reset();

        void addField(ConfiguratorField *field) {
            _fields.push_back(field);
        }

        void addMenuItem(const std::string &path, const std::string &label, bool admin = true);

        void setAdminPassword(const std::string &pw);

        [[nodiscard]] bool isRequestAuthenticated() const;

        void requestAuthentication();

        [[nodiscard]] std::string renderPage(const std::string &title,
                                             const std::string &body,
                                             const std::string &extraHead = "",
                                             bool backButton = true) const;

        [[nodiscard]] std::string renderRedirectToHomePage(const std::string &message) const;

    protected:
        [[nodiscard]] std::string
        _renderConfigPage(bool hasValidationError = false) const;

        [[nodiscard]] std::string _renderHomePage();

        static std::string _generateDeviceName();

        std::string _deviceName = _generateDeviceName();
        WebServer _server;

    private:
        std::vector<ConfiguratorField *> _fields;

        // Menu system: list of navigation items.
        struct MenuItem {
            std::string path;
            std::string label;
            bool admin;
        };

        std::vector<MenuItem> _menu;

        [[nodiscard]] bool _saveConfigFromForm() const;

        static void _erasePreferences();

        std::string _customHomePageHtml;

        std::string _adminPw;
    };
} // namespace SimpleEspConfigurator
