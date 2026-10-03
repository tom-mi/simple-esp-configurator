#pragma once
#include <cmath>
#include <optional>

#include "EspConfiguratorField.h"
#include <string>
#include <utility>

namespace SimpleEspConfigurator {

template<typename T>
class NumberField : public EspConfiguratorField {
public:
    NumberField(
        const std::string &id, std::string label, T defaultValue,
        T minValue, T maxValue, std::optional<T> step)
        : EspConfiguratorField(id),
          _label(std::move(label)),
          _defaultValue(defaultValue),
          _minValue(minValue), _maxValue(maxValue), _step(step),
          _value(defaultValue) {
    }

    void setValueFromForm(const std::string &formVal) override = 0;

    bool validateFromForm(const std::string &formVal) override {
        auto value = _parseValue(formVal);
        if (!value.has_value()) {
            return false;
        }
        if (*value < _minValue) {
            return false;
        }
        if (*value > _maxValue) {
            return false;
        }
        return true;
    }

    bool isMultipleOfStepFromBase(T value, T stepBase, T step) {
        if (step <= 0) {
            return false;
        }
        T diff = value - stepBase;
        T remainder = std::fmod(diff, step);
        return remainder == 0 || remainder == -step;
    }


    std::string generateHtml() override {
        std::string out;
        out += "<label for='" + _id + "'>" + _label + "</label>";
        out += "<input type='number' id='" + _id + "' name='" + _id + "' ";
        out += "value='" + std::to_string(_value) + "' ";
        out += "min='" + std::to_string(_minValue) + "' ";
        out += "max='" + std::to_string(_maxValue) + "' ";
        if (_step.has_value()) {
            out += "step='" + std::to_string(*_step) + "' ";
        } else {
            out += "step='any' ";
        }
        out += ">\n";
        return out;
    };

    T getValue() { return _value; }

    void loadValue() override = 0;

protected:
    virtual std::optional<T> _parseValue(const std::string &formVal) = 0;

    const std::string _label;
    const T _defaultValue;
    const T _minValue;
    const T _maxValue;
    const std::optional<T> _step;
    T _value;
};


class DoubleField : public NumberField<double> {
public:
    using NumberField::NumberField;
    using NumberField::getValue;


    void setValueFromForm(const std::string &formVal) override;

    void loadValue() override;

protected:
    std::optional<double> _parseValue(const std::string &formVal) override;
};

class LongField : public NumberField<long> {
public:
    using NumberField::NumberField;
    using NumberField::getValue;


    void setValueFromForm(const std::string &formVal) override;

    void loadValue() override;

protected:
    std::optional<long> _parseValue(const std::string &formVal) override;
};

class ULongField : public NumberField<unsigned long> {
public:
    using NumberField::NumberField;
    using NumberField::getValue;


    void setValueFromForm(const std::string &formVal) override;

    void loadValue() override;

protected:
    std::optional<unsigned long> _parseValue(const std::string &formVal) override;
};

} // namespace SimpleEspConfigurator
