#pragma once
#include <DNSServer.h>
#include "ConfiguratorCore.h"

namespace SimpleEspConfigurator {
    struct WiFiParameters {
        std::string ssid;
        std::string password;
    };

    struct ApParameters {
        std::string ssid;
        std::string password;
    };

    struct NetworkParameters {
        bool dhcp;
        IPAddress ip;
        IPAddress gateway;
        IPAddress subnet;
        IPAddress dns1;
        IPAddress dns2;
    };

    class Configurator : public ConfiguratorCore {
    public:
        template<TemplateString id, TemplateString label, TemplateString defaultValue = "",
            int minLen = -1, int maxLen = INT_MAX, TemplateString pattern = "", bool required = true>
        [[nodiscard]] std::string stringParameter() {
            auto const field = new StringField(
                id.c_str(), label.c_str(), defaultValue.c_str(), minLen, maxLen, false, pattern.c_str(), required);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        template<TemplateString id, TemplateString label, TemplateString defaultValue = "",
            int minLen = -1, int maxLen = -1, bool required = true>
        std::string passwordParameter() {
            auto const field = new StringField(id.c_str(), label.c_str(), defaultValue.c_str(), minLen,
                                               maxLen, true, "", required);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        template<TemplateString id, TemplateString label, bool defaultValue>
        bool boolParameter() {
            auto const field = new BooleanField(id.c_str(), label.c_str(), defaultValue);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        template<TemplateString id, TemplateString label, double defaultValue, double minValue, double maxValue>
        double doubleParameter() {
            auto const field = new DoubleField(id.c_str(), label.c_str(), defaultValue,
                                               minValue, maxValue, std::nullopt);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        template<TemplateString id, TemplateString label, double defaultValue,
            double minValue, double maxValue, double step>
        double doubleParameter() {
            auto const field = new DoubleField(id.c_str(), label.c_str(), defaultValue,
                                               minValue, maxValue, step);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

        template<TemplateString id, TemplateString label, long defaultValue, long minValue, long maxValue>
        long longParameter() {
            auto const field = new LongField(id.c_str(), label.c_str(), defaultValue,
                                             minValue, maxValue, std::nullopt);
            addField(field);
            field->loadValue();
            return field->getValue();
        }

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
         * @endlink, adds it to the internal field list and returns its current string
         * value.  The caller can subsequently cast the returned string to @link
         * IPAddress @endlink by invoking the field's @code getValue() @endcode
         * method.
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
         * adds it to the internal field list and returns an empty string. The user
         * can optionally provide a callback that receives the raw form value.
         */
        template<TemplateString id, TemplateString html>
        void customHtmlParameter() {
            auto const field = new CustomHtmlField(id.c_str(), html.c_str());
            addField(field);
            // field->loadValue(); // not loading, as that has no effect
        }

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

        template<TemplateString defaultSSID = "", TemplateString defaultPassword = "">
        ApParameters apParameters() {
            const auto apSsid = stringParameter<"ap_ssid", "AP SSID", defaultSSID, 0, 32>();
            return ApParameters(
                // fall back to device name if ssid is empty
            apSsid.empty() ? _deviceName : apSsid,
                passwordParameter<"ap_pass", "AP Password (empty for no encryption)", defaultPassword, 8, 63, false>()
            );
        }

        template<
            bool defaultDhcp = true,
            const TemplateString idPrefix = "wifi_",
            const TemplateString label = "WiFi",
            const TemplateString defaultIp = "192.168.0.42",
            const TemplateString defaultGateway = "192.168.0.1",
            const TemplateString defaultSubnet = "255.255.255.0",
            const TemplateString defaultDns1 = "",
            const TemplateString defaultDns2 = ""
        >
        NetworkParameters networkParameters() {
            return NetworkParameters(
                boolParameter<idPrefix + "dhcp", label + " DHCP", defaultDhcp>(),
                ipv4Parameter<idPrefix + "ip", label + " IP", defaultIp>(),
                ipv4Parameter<idPrefix + "gw", label + " Gateway", defaultGateway>(),
                ipv4Parameter<idPrefix + "sub", label + " Subnet", defaultSubnet>(),
                ipv4Parameter<idPrefix + "dns1", label + " DNS1", defaultDns1, false>(),
                ipv4Parameter<idPrefix + "dns2", label + " DNS2", defaultDns2, false>()
            );
        }

        static bool startAp(const ApParameters &parameters);

        bool startApWithCaptivePortal(const ApParameters &parameters, DNSServer &dnsServer);

        void startCaptivePortal(DNSServer &dnsServer);

        void loop() override;

        void enableOtaUpdate();

    private:
        bool _isOtaUpdateEnabled = true;

        // Page rendering for OTA upload form.
        [[nodiscard]] std::string _renderOtaPage() const;

        // Upload data chunk handler for OTA.
        void _handleOtaUpload();

        // Finalize OTA upload and reboot.
        void _handleOtaFinalize();
    };

    constexpr auto DEFAULT_TIMEOUT_MS = 10000;

    bool connectToWifiSync(const WiFiParameters &parameters, uint16_t timeoutMs = DEFAULT_TIMEOUT_MS);

    bool connectToWifiSync(const WiFiParameters &parameters, const NetworkParameters &netParams,
                           uint16_t timeoutMs = DEFAULT_TIMEOUT_MS);
} // namespace SimpleEspConfigurator
