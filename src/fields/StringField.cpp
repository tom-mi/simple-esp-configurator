#include "StringField.h"
#include <Preferences.h>
#include <regex.h>

// ReSharper disable once CppUnusedIncludeDirective - needed for log tag
#include <stdexcept>
#include <utility>

#include "../Common.h"

namespace SimpleEspConfigurator {

static Preferences prefs;

StringField::StringField(
    const std::string &id, std::string label,
    const std::string &defaultValue, const int minLen, const int maxLen,
    const bool password, std::string pattern, const bool required)
    : ConfiguratorField(id), _label(std::move(label)), _defaultValue(defaultValue),
      _minLen(minLen), _maxLen(maxLen), _password(password), _pattern(std::move(pattern)),
      _required(required), _value(defaultValue),
      _passwordPlaceholder(minLen > 0 ? std::string(minLen, '*') : "***") {
  if (_minLen > _maxLen) {
    throw std::invalid_argument(
        "minLen must be greater than or equal to maxLen");
  }
  if (_password && !_pattern.empty()) {
    throw std::invalid_argument("Cannot use a pattern on a password field");
  }
}

void StringField::setValueFromForm(
    const std::string &formVal) {
  if (_password && formVal == _passwordPlaceholder) {
    ESP_LOGI(SEC_LOG_TAG, "not storing unchanged password %s", _id.c_str());
    return;
  }
  prefs.begin(SEC_PREF_NS);
  if (_password && !formVal.empty()) { // NOLINT(*-branch-clone)
    ESP_LOGI(SEC_LOG_TAG, "storing %s='<censored>'", _id.c_str());
  } else {
    ESP_LOGI(SEC_LOG_TAG, "storing %s='%s'", _id.c_str(), formVal.c_str());
  }
  prefs.putString(_id.c_str(), formVal.c_str());
  prefs.end();
}

std::string StringField::generateHtml() {
  std::string out = "<label for='" + _id + "'>" + _label + "</label>";
  out += "<input id='" + _id + "' name='" + _id + "' ";
  if (_password) {
    out += "type='password'";
  }
  if (_minLen > 0) {
    out += " minlength='" + std::to_string(_minLen) + "'";
  }
  if (_maxLen > 0) {
    out += " maxlength='" + std::to_string(_maxLen) + "'";
  }
  if (!_pattern.empty()) {
    out += " pattern='" + _pattern + "'";
  }
  if (_required) {
    out += " required";
  }
  if (_password) {
    out += " value='" + (_value.empty() ? "" : _passwordPlaceholder) + "'";
  } else {
    out += " value='" + escapeHtml(_value) + "'";
  }
  out += "/>\n";

  return out;
}

bool StringField::validateFromForm(
    const std::string &formVal) {
  if (_required && formVal.empty()) {
    ESP_LOGW(SEC_LOG_TAG, "Validation error: field '%s' is required but empty",
             _id.c_str());
    return false;
  }
  if (!_required && formVal.empty()) {
    return true;
  }
  if (_minLen > 0 && formVal.size() < static_cast<std::size_t>(_minLen)) {
    ESP_LOGW(SEC_LOG_TAG, "Validation error: field '%s' shorter than minLen %d",
             _id.c_str(), _minLen);
    return false;
  }
  if (_maxLen > 0 && formVal.size() > static_cast<std::size_t>(_maxLen)) {
    ESP_LOGW(SEC_LOG_TAG, "Validation error: field '%s' longer than maxLen %d",
             _id.c_str(), _maxLen);
    return false;
  }
  if (!_pattern.empty()) {
      regex_t rgx;
      if (regcomp(&rgx, ("^"  + _pattern + "$").c_str(), REG_EXTENDED) != 0) {
        return false;
      }
      if (regexec(&rgx, formVal.c_str(), 0, nullptr, 0) != 0) {
        ESP_LOGW(SEC_LOG_TAG,
                 "Validation error: field %s='%s' does not match pattern '%s'",
                 _id.c_str(), formVal.c_str(), _pattern.c_str());
        return false;
      }
  }
  return true;
}

void StringField::loadValue() {
  prefs.begin(SEC_PREF_NS);
  if (prefs.isKey(_id.c_str())) {
    _value = prefs.getString(_id.c_str(), _defaultValue.c_str()).c_str();
    if (_password && !_value.empty()) {
      ESP_LOGI(SEC_LOG_TAG, "loaded value %s='%s'", _id.c_str(),
               _passwordPlaceholder.c_str());
    } else {
      ESP_LOGI(SEC_LOG_TAG, "loaded value %s='%s'", _id.c_str(),
               _value.c_str());
    }
  } else {
    ESP_LOGI(SEC_LOG_TAG, "using default value %s='%s'", _id.c_str(),
             _value.c_str());
  }
  prefs.end();
}

} // namespace SimpleEspConfigurator