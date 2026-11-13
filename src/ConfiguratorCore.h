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
        /**
         * @brief Construct a ConfiguratorCore instance.
         *
         * Initializes the configurator and adds the default configuration menu item.
         */
        ConfiguratorCore() { addMenuItem("/config", "Configuration"); }

        virtual ~ConfiguratorCore() = default;

        /**
         * @brief Set the device name.
         * @param name The device name to display in the UI.
         */
        void deviceName(const std::string &name) { _deviceName = name; }
        /**
         * @brief Set custom HTML to prepend to the rendered home page body.
         * @param html The HTML content to prepend.
         */
        void customHomePageHtml(const std::string &html) {
            _customHomePageHtml = html;
        }

        /**
         * @brief Set the footer HTML to append to the rendered page body.
         * @param html The HTML content to append as footer.
         */
        void footer(const std::string &html) {
            _footer = html;
        }

        /**
         * @brief Get the underlying WebServer instance.
         * @return Pointer to the WebServer.
         */
        WebServer *server() { return &_server; };

        /**
         * @brief Start the web server.
         *
         * Initializes and starts the HTTP server for serving configuration pages.
         */
        void startWebServer();

        /**
         * @brief Handle web server requests.
         *
         * Must be called periodically (e.g., in the main loop) to process incoming HTTP requests.
         */
        virtual void loop();

        /**
         * @brief Reboot the ESP device.
         */
        void reboot();

        /**
         * @brief Reset stored configuration and reboot.
         */
        void reset();

        /**
         * @brief Add a configuration field.
         * @param field Pointer to the field to add.
         */
        void addField(ConfiguratorField *field) {
            _fields.push_back(field);
        }

        /**
         * @brief Add a menu item to the navigation.
         * @param path The URL path for the menu item.
         * @param label The display label for the menu item.
         * @param admin Whether this menu item requires admin authentication (default: true).
         */
        void addMenuItem(const std::string &path, const std::string &label, bool admin = true);

        /**
         * @brief Set the admin password for authentication.
         * @param pw The password to require for admin access.
         */
        void setAdminPassword(const std::string &pw);

        /**
         * @brief Check if the current request is authenticated.
         * @return True if the request is authenticated, false otherwise.
         */
        [[nodiscard]] bool isRequestAuthenticated() const;

        /**
         * @brief Request authentication for the current request.
         *
         * Sends an HTTP 401 response with WWW-Authenticate header to prompt for credentials.
         */
        void requestAuthentication();

        /**
         * @brief Render a complete HTML page.
         * @param title The page title.
         * @param body The HTML body content.
         * @param extraHead Additional HTML to include in the head section (default: empty).
         * @param backButton Whether to include a back button (default: true).
         * @return The rendered HTML page as a string.
         */
        [[nodiscard]] std::string renderPage(const std::string &title,
                                             const std::string &body,
                                             const std::string &extraHead = "",
                                             bool backButton = true) const;

        /**
         * @brief Render a redirect page to the home page.
         * @param message The message to display before redirecting.
         * @return The rendered HTML redirect page as a string.
         */
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

        std::string _footer;

        std::string _adminPw;
    };
} // namespace SimpleEspConfigurator
