#pragma once
#include "ConfiguratorField.h"

namespace SimpleEspConfigurator {
    /**
     * @brief Custom HTML field for arbitrary content.
     *
     * The field renders a provided HTML snippet. When a form value is posted for
     * this field, the optional callback is invoked with the raw value.
     */
    class CustomHtmlField : public ConfiguratorField {
    public:
        CustomHtmlField(const std::string &id, std::string html)
            : ConfiguratorField(id), _html(std::move(html)) {
        }

        void setValueFromForm(const std::string &formVal) override {
        }

        bool validateFromForm(const std::string &) override { return true; }

        std::string generateHtml() override { return _html; }

        void loadValue() override {
        }

    private:
        std::string _html;
    };
} // namespace SimpleEspConfigurator
