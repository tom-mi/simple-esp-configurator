#pragma once
#include "EspConfiguratorField.h"
#include <string>

namespace SimpleEspConfigurator {

class BooleanField : public EspConfiguratorField {
public:
    const std::string TRUE_VALUE = "T";

    BooleanField(const std::string &id,
                  std::string label,
                  bool defaultValue);

    void setValueFromForm(const std::string &formVal) override;

    bool validateFromForm(const std::string &formVal) override;

    std::string generateHtml() override;

    [[nodiscard]] bool getValue() const { return _value; }

    void loadValue() override;

protected:
    const std::string _label;
    const bool _defaultValue;
    bool _value;
};

} // namespace SimpleEspConfigurator