#include "NumberField.h"

#include <Preferences.h>

#include "../Common.h"

namespace SimpleEspConfigurator {

static Preferences prefs;

std::optional<double> DoubleField::_parseValue(const std::string &formVal) {
    try {
        return std::stod(formVal);
    } catch (...) {
        return std::nullopt;
    }
}

void DoubleField::setValueFromForm(const std::string &formVal) {
    prefs.begin(SEC_PREF_NS);
    const double val = std::stod(formVal);
    ESP_LOGI(SEC_LOG_TAG, "storing %s=%f", _id.c_str(), val);
    prefs.putDouble(_id.c_str(), val);
    prefs.end();
}

void DoubleField::loadValue() {
    prefs.begin(SEC_PREF_NS);
    if (prefs.isKey(_id.c_str())) {
        _value = prefs.getDouble(_id.c_str(), _defaultValue);
        ESP_LOGI(SEC_LOG_TAG, "loaded value %s=%f", _id.c_str(), _value);
    } else {
        ESP_LOGI(SEC_LOG_TAG, "using default value %s=%f", _id.c_str(), _value);
    }
    prefs.end();
}

std::optional<long> LongField::_parseValue(const std::string &formVal) {
    try {
        return std::stol(formVal);
    } catch (...) {
        return std::nullopt;
    }
}

void LongField::setValueFromForm(const std::string &formVal) {
    prefs.begin(SEC_PREF_NS);
    const long val = std::stol(formVal);
    ESP_LOGI(SEC_LOG_TAG, "storing %s=%ld", _id.c_str(), val);
    prefs.putLong(_id.c_str(), val);
    prefs.end();
}

void LongField::loadValue() {
    prefs.begin(SEC_PREF_NS);
    if (prefs.isKey(_id.c_str())) {
        _value = prefs.getLong(_id.c_str(), _defaultValue);
        ESP_LOGI(SEC_LOG_TAG, "loaded value %s=%ld", _id.c_str(), _value);
    } else {
        ESP_LOGI(SEC_LOG_TAG, "using default value %s=%ld", _id.c_str(), _value);
    }
    prefs.end();
}

std::optional<unsigned long> ULongField::_parseValue(const std::string &formVal) {
    try {
        return std::stoul(formVal);
    } catch (...) {
        return std::nullopt;
    }
}

void ULongField::setValueFromForm(const std::string &formVal) {
    prefs.begin(SEC_PREF_NS);
    const unsigned long val = std::stoul(formVal);
    ESP_LOGI(SEC_LOG_TAG, "storing %s=%lu", _id.c_str(), val);
    prefs.putULong(_id.c_str(), val);
    prefs.end();
}

void ULongField::loadValue() {
    prefs.begin(SEC_PREF_NS);
    if (prefs.isKey(_id.c_str())) {
        _value = prefs.getULong(_id.c_str(), _defaultValue);
        ESP_LOGI(SEC_LOG_TAG, "loaded value %s=%lu", _id.c_str(), _value);
    } else {
        ESP_LOGI(SEC_LOG_TAG, "using default value %s=%lu", _id.c_str(), _value);
    }
    prefs.end();
  }

} // namespace SimpleEspConfigurator