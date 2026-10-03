import os

Import("env")

_KEYS = [
    'WIFI_SSID',
    'WIFI_PASS',
    'WIFI_IP',
    'WIFI_SUBNET',
    'WIFI_GW',
]


def _fail():
    print('Please create a .wifi.conf file in the repository root with the content')
    for k in _KEYS:
        print(k + '=<...>')
    raise ValueError('.wifi.conf missing')


if not os.path.exists('.wifi.conf'):
    _fail()

cpp_defines = []
with open('.wifi.conf', 'r') as f:
    for line in f:
        key, value = line.strip().split('=', maxsplit=1)
        if key in _KEYS:
            cpp_defines.append((key, env.StringifyMacro(value)))

env.Append(CPPDEFINES=cpp_defines)
