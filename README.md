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
| [basic-wifi](./examples/basic-wifi) | 1048208 | +123216 |
| [minimal](./examples/minimal) | 1009328 | +84336 |
<!-- END example table -->

## Configuration reference

