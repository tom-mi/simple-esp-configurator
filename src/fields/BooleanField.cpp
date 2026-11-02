#include "BooleanField.h"
#include <Preferences.h>

#include <utility>

// ReSharper disable once CppUnusedIncludeDirective - needed for log tag
#include "../Common.h"

namespace SimpleEspConfigurator {
    static Preferences prefs;

    BooleanField::BooleanField(
        const std::string &id, std::string label, const bool defaultValue)
        : ConfiguratorField(id), _label(std::move(label)), _defaultValue(defaultValue),
          _value(defaultValue) {
    }

    void BooleanField::setValueFromForm(
        const std::string &formVal) {
        const bool newValue = (formVal == std::string(TRUE_VALUE));
        prefs.begin(SEC_PREF_NS);
        ESP_LOGI(SEC_LOG_TAG, "storing %s=%s", _id.c_str(), formatBool(newValue));
        prefs.putBool(_id.c_str(), newValue);
        prefs.end();
    }

    bool BooleanField::validateFromForm(
        const std::string &formVal) {
        return true;
    }

    std::string BooleanField::generateHtml() {
        std::string out = "<input type='checkbox' id='" + _id + "' name='" + _id +
                          "' value='" + TRUE_VALUE + "' ";
        if (_value) {
            out += "checked ";
        }
        out += ">\n<label for='" + _id + "'>" + _label + "</label>\n";
        return out;
    }

    void BooleanField::loadValue() {
        prefs.begin(SEC_PREF_NS);
        if (prefs.isKey(_id.c_str())) {
            _value = prefs.getBool(_id.c_str(), _defaultValue);
            ESP_LOGI(SEC_LOG_TAG, "loaded value %s=%s", _id.c_str(),
                     formatBool(_value));
        } else {
            ESP_LOGI(SEC_LOG_TAG, "Using default value %s=%s", _id.c_str(),
                     formatBool(_value));
        }
        prefs.end();
    }
} // namespace SimpleEspConfigurator
