#pragma once
#include "StringField.h"

namespace SimpleEspConfigurator {

/**
 * @brief Specialisation of StringField for IPv4 address configuration.
 *
 * This field stores the value as a string internally but exposes a
 * helper `getValue()` that returns the value as an `IPAddress` object.
 *
 * The constructor forwards to `StringField` with
 * `password` flag set to `false` and passes along the supplied pattern
 * (useful for validating IPv4 addresses).  No additional validation
 * logic is added – the pattern supplied by the caller should be a
 * regex matching IPv4 addresses.
 */
class IPv4AddressField
    : public StringField {
public:
  IPv4AddressField(const std::string &id,
                    const std::string &label,
                    const std::string &defaultValue,
                    const bool required = true)
      : StringField(
            id, label, defaultValue,
            -1, -1, false, "",
            required) {}

  /**
   * @return IP address parsed from the stored string value.
   */
  [[nodiscard]] IPAddress getIpValue() const { return {_value.c_str()}; }
};

} // namespace SimpleEspConfigurator