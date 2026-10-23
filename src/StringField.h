#pragma once
#include "ConfiguratorField.h"
#include <string>

namespace SimpleEspConfigurator {

class StringField : public ConfiguratorField {
public:
    StringField(
        const std::string &id, std::string label,
        const std::string &defaultValue, int minLen, int maxLen,
        bool password, std::string pattern, bool required);

    void setValueFromForm(const std::string &formVal) override;

    bool validateFromForm(const std::string &formVal) override;

    std::string generateHtml() override;

    std::string getValue() { return _value; }

    void loadValue() override;

protected:
    const std::string _label;
    const std::string _defaultValue;
    const int _minLen;
    const int _maxLen;
    const bool _password;
    const std::string _pattern;
    const bool _required;
    std::string _value;

private:
    const std::string _passwordPlaceholder;
};

} // namespace SimpleEspConfigurator