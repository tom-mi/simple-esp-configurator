# simple-esp-configurator

Yet another configuration library for ESP32 / ESP8266 devices.

Features & philosophy:
* Covers the typical use case: Wi-Fi setup via AP & config web portal
* Keep it simple – load parameters once, reboot on save
* Form validation via HTML input attributes
* Backend validation matching the frontend constraints
* Static (build-time) validation of setup where possible
* No magic (almost)
* Modular – use the parts you need, omit the rest
* Most parts E2E tested with playwright

## Installation

TODO

## Usage

See the [examples](./examples):

<!-- START example table -->
| Example | Size (bytes) | Delta to baseline (bytes) |
|---------|--------------:|---------------------------:|
| [basic-wifi](./examples/basic-wifi) | 1052752 | +127712 |
| [minimal](./examples/minimal) | 1011136 | +86096 |
| [minimal-wifi](./examples/minimal-wifi) | 1016144 | +91104 |
<!-- END example table -->

## Configuration reference

### ConfiguratorCore

| Method | Description |
|--------|-------------|
| `deviceName(name)` | Set the device name to display in the UI. |
| `customHomePageHtml(html)` | Set custom HTML to prepend to the rendered home page body. |
| `server()` | Get the underlying WebServer instance. |
| `startWebServer()` | Start the web server for serving configuration pages. |
| `loop()` | Handle web server requests. Must be called periodically in the main loop. |
| `reboot()` | Reboot the ESP device. |
| `reset()` | Reset stored configuration and reboot. |
| `addField(field)` | Add a configuration field. |
| `addMenuItem(path, label, admin=true)` | Add a menu item to the navigation. |
| `setAdminPassword(pw)` | Set the admin password for authentication. |
| `isRequestAuthenticated()` | Check if the current request is authenticated. |
| `requestAuthentication()` | Request authentication for the current request (sends HTTP 401). |
| `renderPage(title, body, extraHead="", backButton=true)` | Render a complete HTML page. |
| `renderRedirectToHomePage(message)` | Render a redirect page to the home page. |

### Configurator

| Method | Description |
|--------|-------------|
| `stringParameter<id, label, defaultValue, minLen, maxLen, pattern, required>()` | Create a string configuration field. Returns the current string value. |
| `passwordParameter<id, label, defaultValue, minLen, maxLen, required>()` | Create a password configuration field. Returns the current string value. |
| `boolParameter<id, label, defaultValue>()` | Create a boolean configuration field. Returns the current boolean value. |
| `doubleParameter<id, label, defaultValue, minValue, maxValue>()` | Create a double configuration field. Returns the current double value. |
| `doubleParameter<id, label, defaultValue, minValue, maxValue, step>()` | Create a double configuration field with step. Returns the current double value. |
| `longParameter<id, label, defaultValue, minValue, maxValue>()` | Create a long configuration field. Returns the current long value. |
| `longParameter<id, label, defaultValue, minValue, maxValue, step>()` | Create a long configuration field with step. Returns the current long value. |
| `ulongParameter<id, label, defaultValue, minValue, maxValue>()` | Create an unsigned long configuration field. Returns the current value. |
| `ulongParameter<id, label, defaultValue, minValue, maxValue, step>()` | Create an unsigned long configuration field with step. Returns the current value. |
| `ipv4Parameter<id, label, defaultValue, required>()` | Create an IPv4 address configuration field. Returns the current IPAddress. |
| `customHtmlParameter<id, html>()` | Create a custom HTML field that renders the provided HTML snippet. |
| `wifiParameters<defaultSSID, defaultPassword, idPrefix, label>()` | Create WiFi configuration parameters (SSID and password). |
| `apParameters<defaultSSID, defaultPassword>()` | Create Access Point configuration parameters. |
| `networkParameters<defaultDhcp, idPrefix, label, defaultIp, defaultGateway, defaultSubnet, defaultDns1, defaultDns2, defaultHostname>()` | Create network configuration parameters (DHCP, IP, gateway, subnet, DNS, hostname). |
| `enableOtaUpdate()` | Enable OTA (Over-The-Air) update functionality. |
| `connectToWifiSync(parameters, netParams, timeoutMs)` | Connect to WiFi synchronously with network parameters. |
| `connectToWifiSync(parameters, timeoutMs)` | Connect to WiFi synchronously using DHCP. |
| `startAp(parameters)` | Start the Access Point. |
| `startApWithCaptivePortal(parameters, dnsServer)` | Start the Access Point with captive portal. |
| `startCaptivePortal(dnsServer)` | Start the captive portal. |

## Development & Testing

Build all example firmwares (this also updates the example firmware table above)
```
./scripts/build
```

Run offline tests (currently only tests that firmware sizes don't grow)
```
./scripts/test offline
```

Run ESP32 tests (requires a ESP32 device connected via USB)
```
./scripts/test esp32
```