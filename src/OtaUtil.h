#pragma once
#include "ConfiguratorCore.h"

namespace SimpleEspConfigurator {
    /**
     * @brief Enable OTA (Over-The-Air) update functionality.
     *
     * Sets up the OTA update server and registers the update endpoint.
     * @param configurator The ConfiguratorCore instance.
     */
    void enableOtaUpdate(ConfiguratorCore &configurator);
} // namespace SimpleEspConfigurator
