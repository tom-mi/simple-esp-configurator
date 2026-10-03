#pragma once
#include <string>
#include <utility>
// IPAddress type used for IPv4 configuration values.
// It is defined in the Arduino core headers; including <WiFi.h> pulls it in.

class EspConfiguratorField {
public:
  EspConfiguratorField(std::string id)
      : _id(std::move(id)) {}

  virtual ~EspConfiguratorField() = default;

  const std::string _id;

  virtual void setValueFromForm(const std::string &formVal) = 0;

  virtual bool validateFromForm(const std::string &formVal) = 0;

  virtual std::string generateHtml() = 0;

  virtual void loadValue() = 0;

  [[nodiscard]] std::string id() const { return _id; }
};
