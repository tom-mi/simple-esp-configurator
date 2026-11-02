#pragma once
#include "ConfiguratorCore.h"
#include "OtaUtil.h"
#include "WifiUtil.h"
#include "fields/BooleanField.h"
#include "fields/CustomHtmlField.h"
#include "fields/IPv4AddressField.h"
#include "fields/NumberField.h"
#include "fields/StringField.h"

#include "Common.h"

constexpr auto DEFAULT_TIMEOUT_MS = 10000;


namespace SimpleEspConfigurator {
    /**
     * Main entrypoint for the SimpleEspConfigurator.
     * Subclasses ConfiguratorCore to provide higher-level methods for easy setup and additional features.
     */
    class Configurator : public ConfiguratorCore {
    public:
        /**
         * @brief Factory method for string configuration fields.
         *
         * Creates a StringField, adds it to the internal field list and returns its current value.
         * @tparam id The unique identifier for this field.
         * @tparam label The display label for this field.
         * @tparam defaultValue The default value (default: empty string).
         * @tparam minLen Minimum length constraint (default: -1 for no minimum).
         * @tparam maxLen Maximum length constraint (default: INT_MAX for no maximum).
         * @tparam pattern Regular expression pattern for validation (default: empty for no pattern).
         * @tparam required Whether the field is required (default: true).
         * @return The current string value of the field.
         */
        template<TemplateString id, TemplateString label, TemplateString defaultValue = "",
            int minLen = -1, int maxLen = INT_MAX, TemplateString pattern = "", bool required = true>
        [[nodiscard]] std::string stringParameter() {
            auto const field = new StringField(
                id.c_str(), label.c_str(), defaultValue.c_str(), minLen, maxLen, false, pattern.c_str(), required);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        /**
         * @brief Factory method for password configuration fields.
         *
         * Creates a StringField with password input type, adds it to the internal field list and returns its current value.
         * @tparam id The unique identifier for this field.
         * @tparam label The display label for this field.
         * @tparam defaultValue The default value (default: empty string).
         * @tparam minLen Minimum length constraint (default: -1 for no minimum).
         * @tparam maxLen Maximum length constraint (default: -1 for no maximum).
         * @tparam required Whether the field is required (default: true).
         * @return The current string value of the field.
         */
        template<TemplateString id, TemplateString label, TemplateString defaultValue = "",
            int minLen = -1, int maxLen = -1, bool required = true>
        std::string passwordParameter() {
            auto const field = new StringField(id.c_str(), label.c_str(), defaultValue.c_str(), minLen,
                                               maxLen, true, "", required);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        /**
         * @brief Factory method for boolean configuration fields.
         *
         * Creates a BooleanField, adds it to the internal field list and returns its current value.
         * @tparam id The unique identifier for this field.
         * @tparam label The display label for this field.
         * @tparam defaultValue The default value.
         * @return The current boolean value of the field.
         */
        template<TemplateString id, TemplateString label, bool defaultValue>
        bool boolParameter() {
            auto const field = new BooleanField(id.c_str(), label.c_str(), defaultValue);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        /**
         * @brief Factory method for double configuration fields.
         *
         * Creates a DoubleField, adds it to the internal field list and returns its current value.
         * @tparam id The unique identifier for this field.
         * @tparam label The display label for this field.
         * @tparam defaultValue The default value.
         * @tparam minValue The minimum allowed value.
         * @tparam maxValue The maximum allowed value.
         * @return The current double value of the field.
         */
        template<TemplateString id, TemplateString label, double defaultValue, double minValue, double maxValue>
        double doubleParameter() {
            auto const field = new DoubleField(id.c_str(), label.c_str(), defaultValue,
                                               minValue, maxValue, std::nullopt);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        /**
         * @brief Factory method for double configuration fields with step.
         *
         * Creates a DoubleField with a step constraint, adds it to the internal field list and returns its current value.
         * @tparam id The unique identifier for this field.
         * @tparam label The display label for this field.
         * @tparam defaultValue The default value.
         * @tparam minValue The minimum allowed value.
         * @tparam maxValue The maximum allowed value.
         * @tparam step The step size for the input.
         * @return The current double value of the field.
         */
        template<TemplateString id, TemplateString label, double defaultValue,
            double minValue, double maxValue, double step>
        double doubleParameter() {
            auto const field = new DoubleField(id.c_str(), label.c_str(), defaultValue,
                                               minValue, maxValue, step);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        /**
         * @brief Factory method for long configuration fields.
         *
         * Creates a LongField, adds it to the internal field list and returns its current value.
         * @tparam id The unique identifier for this field.
         * @tparam label The display label for this field.
         * @tparam defaultValue The default value.
         * @tparam minValue The minimum allowed value.
         * @tparam maxValue The maximum allowed value.
         * @return The current long value of the field.
         */
        template<TemplateString id, TemplateString label, long defaultValue, long minValue, long maxValue>
        long longParameter() {
            auto const field = new LongField(id.c_str(), label.c_str(), defaultValue,
                                             minValue, maxValue, std::nullopt);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        /**
         * @brief Factory method for long configuration fields with step.
         *
         * Creates a LongField with a step constraint, adds it to the internal field list and returns its current value.
         * @tparam id The unique identifier for this field.
         * @tparam label The display label for this field.
         * @tparam defaultValue The default value.
         * @tparam minValue The minimum allowed value.
         * @tparam maxValue The maximum allowed value.
         * @tparam step The step size for the input.
         * @return The current long value of the field.
         */
        template<TemplateString id, TemplateString label, long defaultValue,
            long minValue,
            long maxValue,
            long step>
        long longParameter() {
            auto const field = new LongField(id.c_str(), label.c_str(), defaultValue,
                                             minValue, maxValue, step);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        /**
         * @brief Factory method for unsigned long configuration fields.
         *
         * Creates a ULongField, adds it to the internal field list and returns its current value.
         * @tparam id The unique identifier for this field.
         * @tparam label The display label for this field.
         * @tparam defaultValue The default value.
         * @tparam minValue The minimum allowed value.
         * @tparam maxValue The maximum allowed value.
         * @return The current unsigned long value of the field.
         */
        template<TemplateString id, TemplateString label, unsigned long defaultValue,
            unsigned long minValue,
            unsigned long maxValue>
        unsigned long ulongParameter() {
            auto const field = new ULongField(id.c_str(), label.c_str(), defaultValue,
                                              minValue, maxValue, std::nullopt);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        /**
         * @brief Factory method for unsigned long configuration fields with step.
         *
         * Creates a ULongField with a step constraint, adds it to the internal field list and returns its current value.
         * @tparam id The unique identifier for this field.
         * @tparam label The display label for this field.
         * @tparam defaultValue The default value.
         * @tparam minValue The minimum allowed value.
         * @tparam maxValue The maximum allowed value.
         * @tparam step The step size for the input.
         * @return The current unsigned long value of the field.
         */
        template<TemplateString id, TemplateString label, unsigned long defaultValue,
            unsigned long minValue, unsigned long maxValue, unsigned long step>
        unsigned long ulongParameter() {
            auto const field = new ULongField(id.c_str(), label.c_str(), defaultValue,
                                              minValue, maxValue, step);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        /**
         * @brief Factory method for IPv4 address configuration.
         *
         * Creates an instance of @link IPv4AddressField
         * @endlink, adds it to the internal field list and returns the current value
         * as an @link IPAddress @endlink.
         * @tparam _id The unique identifier for this field.
         * @tparam _label The display label for this field.
         * @tparam _defaultValue The default IP address value.
         * @tparam required Whether the field is required (default: true).
         * @return The current IP address value.
         */
        template<TemplateString _id, TemplateString _label, TemplateString _defaultValue, bool required = true>
        [[nodiscard]] IPAddress ipv4Parameter() {
            auto const field = new IPv4AddressField(_id.c_str(), _label.c_str(), _defaultValue.c_str(),
                                                    required);
            addField(field);
            field->loadValue();
            return field->getIpValue();
        }

        /**
         * @brief Factory method for custom HTML fields.
         *
         * Creates an instance of @link CustomHtmlField @endlink,
         * adds it to the internal field list. The field renders the provided HTML
         * snippet directly on the configuration page.
         * @tparam id The unique identifier for this field.
         * @tparam html The HTML content to render.
         */
        template<TemplateString id, TemplateString html>
        void customHtmlParameter() {
            auto const field = new CustomHtmlField(id.c_str(), html.c_str());
            addField(field);
            // field->loadValue(); // not loading, as that has no effect
        }

        /**
         * @brief Factory method for WiFi configuration parameters.
         *
         * Creates SSID and password fields for WiFi configuration.
         * @tparam defaultSSID The default SSID value (default: empty).
         * @tparam defaultPassword The default password value (default: empty).
         * @tparam idPrefix The prefix for field IDs (default: "wifi_").
         * @tparam label The label for the WiFi section (default: "WiFi").
         * @return WiFiParameters containing the SSID and password.
         */
        template<
            const TemplateString defaultSSID = "",
            const TemplateString defaultPassword = "",
            const TemplateString idPrefix = "wifi_",
            const TemplateString label = "WiFi">
        WiFiParameters wifiParameters() {
            constexpr auto ssidId = idPrefix + "ssid";
            constexpr auto ssidLabel = label + " SSID";
            return WiFiParameters(
                stringParameter<ssidId, ssidLabel, defaultSSID, 0, 32, "", false>(),
                passwordParameter<idPrefix + "pass", label + " Password", defaultPassword, 8, 63, false>()
            );
        }

        /**
         * @brief Factory method for Access Point configuration parameters.
         *
         * Creates SSID and password fields for AP configuration.
         * @tparam defaultSSID The default SSID value (default: empty, falls back to device name).
         * @tparam defaultPassword The default password value (default: empty for no encryption).
         * @return ApParameters containing the SSID and password.
         */
        template<TemplateString defaultSSID = "", TemplateString defaultPassword = "">
        ApParameters apParameters() {
            const auto apSsid = stringParameter<"ap_ssid", "AP SSID", defaultSSID, 0, 32, "", false>();
            return ApParameters(
                // fall back to device name if ssid is empty
                apSsid.empty() ? _deviceName : apSsid,
                passwordParameter<"ap_pass", "AP Password (empty for no encryption)", defaultPassword, 8, 63, false>()
            );
        }

        /**
         * @brief Factory method for network configuration parameters.
         *
         * Creates fields for DHCP, IP address, gateway, subnet, DNS, and hostname configuration.
         * @tparam defaultDhcp Whether DHCP is enabled by default (default: true).
         * @tparam idPrefix The prefix for field IDs (default: "wifi_").
         * @tparam label The label for the network section (default: "WiFi").
         * @tparam defaultIp The default IP address (default: "192.168.0.42").
         * @tparam defaultGateway The default gateway (default: "192.168.0.1").
         * @tparam defaultSubnet The default subnet mask (default: "255.255.255.0").
         * @tparam defaultDns1 The default DNS1 server (default: empty).
         * @tparam defaultDns2 The default DNS2 server (default: empty).
         * @tparam defaultHostname The default DHCP hostname (default: empty).
         * @return NetworkParameters containing all network configuration values.
         */
        template<
            bool defaultDhcp = true,
            const TemplateString idPrefix = "wifi_",
            const TemplateString label = "WiFi",
            const TemplateString defaultIp = "192.168.0.42",
            const TemplateString defaultGateway = "192.168.0.1",
            const TemplateString defaultSubnet = "255.255.255.0",
            const TemplateString defaultDns1 = "",
            const TemplateString defaultDns2 = "",
            const TemplateString defaultHostname = ""
        >
        NetworkParameters networkParameters() {
            return NetworkParameters(
                boolParameter<idPrefix + "dhcp", label + " DHCP", defaultDhcp>(),
                ipv4Parameter<idPrefix + "ip", label + " IP", defaultIp>(),
                ipv4Parameter<idPrefix + "gw", label + " Gateway", defaultGateway>(),
                ipv4Parameter<idPrefix + "sub", label + " Subnet", defaultSubnet>(),
                ipv4Parameter<idPrefix + "dns1", label + " DNS1", defaultDns1, false>(),
                ipv4Parameter<idPrefix + "dns2", label + " DNS2", defaultDns2, false>(),
                stringParameter<idPrefix + "host", label + " DHCP Hostname", defaultHostname, 0, 63, "", false>()
            );
        }

        /*
         * Convenience methods for easy use (even though some of them could be static).
         * The logic is intentionally moved to other files, so the compiler & linker can optimize for smaller binaries.
         */

        /**
         * @brief Enable OTA (Over-The-Air) update functionality.
         *
         * Sets up the OTA update server and registers the update endpoint.
         */
        void enableOtaUpdate() {
            SimpleEspConfigurator::enableOtaUpdate(*this);
        }

        /**
         * @brief Connect to WiFi synchronously with network parameters.
         * @param parameters The WiFi credentials (SSID and password).
         * @param netParams The network configuration (DHCP, IP, gateway, etc.).
         * @param timeoutMs The timeout in milliseconds (default: DEFAULT_TIMEOUT_MS).
         * @return True if connection was successful, false otherwise.
         */
        bool connectToWifiSync(const WiFiParameters &parameters, const NetworkParameters &netParams, // NOLINT(*-convert-member-functions-to-static)
                               const uint16_t timeoutMs = DEFAULT_TIMEOUT_MS) {
            return SimpleEspConfigurator::connectToWifiSync(parameters, netParams, timeoutMs);
        }

        /**
         * @brief Connect to WiFi synchronously using DHCP.
         * @param parameters The WiFi credentials (SSID and password).
         * @param timeoutMs The timeout in milliseconds (default: DEFAULT_TIMEOUT_MS).
         * @return True if connection was successful, false otherwise.
         */
        bool connectToWifiSync(const WiFiParameters &parameters, const uint16_t timeoutMs = DEFAULT_TIMEOUT_MS) { // NOLINT(*-convert-member-functions-to-static)
            const auto netParams = NetworkParameters{.dhcp = true};
            return SimpleEspConfigurator::connectToWifiSync(parameters, netParams, timeoutMs);
        }

        /**
         * @brief Start the Access Point.
         * @param parameters The AP configuration (SSID and password).
         * @return True if AP was started successfully, false otherwise.
         */
        bool startAp(const ApParameters &parameters) { // NOLINT(*-convert-member-functions-to-static)
            return SimpleEspConfigurator::startAp(parameters);
        }

        /**
         * @brief Start the Access Point with captive portal.
         * @param parameters The AP configuration (SSID and password).
         * @param dnsServer The DNS server for the captive portal.
         * @return True if AP was started successfully, false otherwise.
         */
        bool startApWithCaptivePortal(const ApParameters &parameters, DNSServer &dnsServer) {
            return SimpleEspConfigurator::startApWithCaptivePortal(*this, parameters, dnsServer);
        }

        /**
         * @brief Start the captive portal.
         * @param dnsServer The DNS server for the captive portal.
         */
        void startCaptivePortal(DNSServer &dnsServer) {
            SimpleEspConfigurator::startCaptivePortal(*this, dnsServer);
        }
    };
} // namespace SimpleEspConfigurator
