import pytest
from playwright.sync_api import expect


@pytest.fixture
def clean_config(home_page):
    home_page.reset()
    yield
    # todo also reset after test? wastes quite some time though


@pytest.fixture
def default_form_dict():
    return {
        'str1': 'abc',
        'str2': 'Abc',
        'ip1': '127.0.0.1',
        'ip2': '192.168.1.1',
        'double1': '42.0',
        'double2': '3.14',
        'double3': '2.1',
        'long1': '42',
        'long2': '1000',
        'ulong1': '42',
        'ulong2': '1000',
        'p1': 's3cr3t',
        'p2': '',
        'boolean1': 'T',
        'boolean2': '',
        'ap_ssid': 'my_ap_ssid',
        'ap_pass': 'my_ap_pass',
        'wifi1_ssid': 'my_ssid',
        'wifi1_pass': 'my_pass1',
        'net1_dhcp': '',
        'net1_ip': '192.168.10.2',
        'net1_gw': '192.168.10.1',
        'net1_sub': '255.255.0.0',
        'net1_dns1': '1.2.3.4',
        'net1_dns2': '5.6.7.8',
    }


def test_config_page_navigation(home_page, config_page):
    home_page.goto()

    home_page.configuration_button().click()

    config_page.expect_at_page()

    config_page.back_button().click()

    home_page.expect_at_page()


def test_default_values(clean_config, config_page, field_page):
    # given
    config_page.goto()

    # then
    expect(config_page.field('String')).to_have_value('abc')
    expect(config_page.field('Regex')).to_have_value('Abc')
    expect(config_page.field('IP1')).to_have_value('127.0.0.1')
    expect(config_page.field('IP2')).to_have_value('127.0.0.2')
    expect(config_page.field('Boolean1')).to_be_checked()
    expect(config_page.field('Boolean2')).not_to_be_checked()
    expect(config_page.field('Double1')).to_have_value('42.000000')
    expect(config_page.field('Double2')).to_have_value('3.140000')
    expect(config_page.field('Double3')).to_have_value('2.100000')
    expect(config_page.field('Long1')).to_have_value('42')
    expect(config_page.field('Long2')).to_have_value('1000')
    expect(config_page.field('ULong1')).to_have_value('42')
    expect(config_page.field('ULong2')).to_have_value('1000')
    expect(config_page.field('Password1')).to_have_value('****')  # pw placeholder has length minlen if set
    expect(config_page.field('Password2')).to_have_value('')  # empty for empty pw
    # nested ap parameters
    expect(config_page.field('AP SSID')).to_have_value('my_ap_ssid')
    expect(config_page.field('AP Password (empty for no encryption)')).to_have_value('********')
    # nested wifi parameters
    expect(config_page.field('MyWiFi SSID')).to_have_value('my_ssid')
    expect(config_page.field('MyWiFi Password')).to_have_value('********')
    # nested network parameters
    expect(config_page.field('MyNetwork DHCP')).not_to_be_checked()
    expect(config_page.field('MyNetwork IP')).to_have_value('192.168.10.2')
    expect(config_page.field('MyNetwork Gateway')).to_have_value('192.168.10.1')
    expect(config_page.field('MyNetwork Subnet')).to_have_value('255.255.0.0')
    expect(config_page.field('MyNetwork DNS1')).to_have_value('1.2.3.4')
    expect(config_page.field('MyNetwork DNS2')).to_have_value('5.6.7.8')

    # when
    field_page.goto()

    # then
    expect(field_page.field_value('str1')).to_have_text('abc')
    expect(field_page.field_value('str2')).to_have_text('Abc')
    expect(field_page.field_value('ip1')).to_have_text('127.0.0.1')
    expect(field_page.field_value('ip2')).to_have_text('127.0.0.2')
    expect(field_page.field_value('bool1')).to_have_text('1')
    expect(field_page.field_value('bool2')).to_have_text('0')
    expect(field_page.field_value('double1')).to_have_text('42.000000')
    expect(field_page.field_value('double2')).to_have_text('3.140000')
    expect(field_page.field_value('double3')).to_have_text('2.100000')
    expect(field_page.field_value('long1')).to_have_text('42')
    expect(field_page.field_value('long2')).to_have_text('1000')
    expect(field_page.field_value('ulong1')).to_have_text('42')
    expect(field_page.field_value('ulong2')).to_have_text('1000')
    expect(field_page.field_value('p1')).to_have_text('s3cr3t')
    expect(field_page.field_value('p2')).to_have_text('')
    expect(field_page.field_value('ap1_ssid')).to_have_text('my_ap_ssid')
    expect(field_page.field_value('ap1_pass')).to_have_text('my_ap_pass')
    expect(field_page.field_value('wifi1_ssid')).to_have_text('my_ssid')
    expect(field_page.field_value('wifi1_pass')).to_have_text('my_pass1')
    expect(field_page.field_value('net1_dhcp')).to_have_text('0')
    expect(field_page.field_value('net1_ip')).to_have_text('192.168.10.2')
    expect(field_page.field_value('net1_gw')).to_have_text('192.168.10.1')
    expect(field_page.field_value('net1_sub')).to_have_text('255.255.0.0')
    expect(field_page.field_value('net1_dns1')).to_have_text('1.2.3.4')
    expect(field_page.field_value('net1_dns2')).to_have_text('5.6.7.8')


def test_set_values(clean_config, config_page, home_page, field_page):
    # given
    config_page.goto()

    # when
    config_page.field('String').fill('foo')
    config_page.field('Regex').fill('Foo')
    config_page.field('IP1').fill('')
    config_page.field('IP2').fill('192.168.1.1')
    config_page.field('Boolean1').uncheck()
    config_page.field('Boolean2').check()
    config_page.field('Double1').fill('99.9')
    config_page.field('Double2').fill('9.99')
    config_page.field('Double3').fill('4.1')
    config_page.field('Long1').fill('12345')
    config_page.field('Long2').fill('5000')
    config_page.field('ULong1').fill('6789')
    config_page.field('ULong2').fill('1111')
    config_page.field('Password1').fill('abcd')
    config_page.field('Password2').fill('pw')
    # nested ap parameters
    config_page.field('AP SSID').fill('NewAP')
    config_page.field('AP Password (empty for no encryption)').fill('')
    # nested wifi parameters
    config_page.field('MyWiFi SSID').fill('NewWiFi')
    config_page.field('MyWiFi Password').fill('NewPassABC')
    # nested network parameters
    config_page.field('MyNetwork DHCP').check()
    config_page.field('MyNetwork IP').fill('10.0.0.2')
    config_page.field('MyNetwork Gateway').fill('10.0.0.1')
    config_page.field('MyNetwork Subnet').fill('255.255.255.0')
    config_page.field('MyNetwork DNS1').fill('8.8.8.8')
    config_page.field('MyNetwork DNS2').fill('8.8.4.4')
    config_page.submit_button().click()

    # then
    home_page.expect_reboot_dialogue()
    home_page.expect_at_page()

    # when
    config_page.goto()

    # then
    expect(config_page.field('String')).to_have_value('foo')
    expect(config_page.field('Regex')).to_have_value('Foo')
    expect(config_page.field('IP1')).to_have_value('')
    expect(config_page.field('IP2')).to_have_value('192.168.1.1')
    expect(config_page.field('Boolean1')).not_to_be_checked()
    expect(config_page.field('Boolean2')).to_be_checked()
    expect(config_page.field('Password1')).to_have_value('****')  # pw placeholder has length minlen if set
    expect(config_page.field('Password2')).to_have_value('***')  # '***' if no minlen is set
    expect(config_page.field('Double1')).to_have_value('99.900000')
    expect(config_page.field('Double2')).to_have_value('9.990000')
    expect(config_page.field('Double3')).to_have_value('4.100000')
    expect(config_page.field('Long1')).to_have_value('12345')
    expect(config_page.field('Long2')).to_have_value('5000')
    expect(config_page.field('ULong1')).to_have_value('6789')
    expect(config_page.field('ULong2')).to_have_value('1111')
    # nested ap parameters
    expect(config_page.field('AP SSID')).to_have_value('NewAP')
    expect(config_page.field('AP Password (empty for no encryption)')).to_have_value('')
    # nested wifi parameters
    expect(config_page.field('MyWiFi SSID')).to_have_value('NewWiFi')
    expect(config_page.field('MyWiFi Password')).to_have_value('********')
    # nested network parameters
    expect(config_page.field('MyNetwork DHCP')).to_be_checked()
    expect(config_page.field('MyNetwork IP')).to_have_value('10.0.0.2')
    expect(config_page.field('MyNetwork Gateway')).to_have_value('10.0.0.1')
    expect(config_page.field('MyNetwork Subnet')).to_have_value('255.255.255.0')
    expect(config_page.field('MyNetwork DNS1')).to_have_value('8.8.8.8')
    expect(config_page.field('MyNetwork DNS2')).to_have_value('8.8.4.4')

    # when
    field_page.goto()

    # then
    expect(field_page.field_value('str1')).to_have_text('foo')
    expect(field_page.field_value('str2')).to_have_text('Foo')
    expect(field_page.field_value('ip1')).to_have_text('0.0.0.0')  # empty IP field creates this IPAddress value
    expect(field_page.field_value('ip2')).to_have_text('192.168.1.1')
    expect(field_page.field_value('bool1')).to_have_text('0')
    expect(field_page.field_value('bool2')).to_have_text('1')
    expect(field_page.field_value('double1')).to_have_text('99.900000')
    expect(field_page.field_value('double2')).to_have_text('9.990000')
    expect(field_page.field_value('double3')).to_have_text('4.100000')
    expect(field_page.field_value('long1')).to_have_text('12345')
    expect(field_page.field_value('long2')).to_have_text('5000')
    expect(field_page.field_value('ulong1')).to_have_text('6789')
    expect(field_page.field_value('ulong2')).to_have_text('1111')
    expect(field_page.field_value('p1')).to_have_text('abcd')
    expect(field_page.field_value('p2')).to_have_text('pw')
    # nested ap parameters
    expect(field_page.field_value('ap1_ssid')).to_have_text('NewAP')
    expect(field_page.field_value('ap1_pass')).to_have_text('')
    # nested wifi parameters
    expect(field_page.field_value('wifi1_ssid')).to_have_text('NewWiFi')
    expect(field_page.field_value('wifi1_pass')).to_have_text('NewPassABC')
    # nested network parameters
    expect(field_page.field_value('net1_dhcp')).to_have_text('1')
    expect(field_page.field_value('net1_ip')).to_have_text('10.0.0.2')
    expect(field_page.field_value('net1_gw')).to_have_text('10.0.0.1')
    expect(field_page.field_value('net1_sub')).to_have_text('255.255.255.0')
    expect(field_page.field_value('net1_dns1')).to_have_text('8.8.8.8')
    expect(field_page.field_value('net1_dns2')).to_have_text('8.8.4.4')


@pytest.mark.parametrize('value', [
    "'/>Test Test<span class='",
    '"/>Test Test<span class="',
    '\'"/>Test Test<span class="\'',
    '&amp;',
])
def test_html_injection(clean_config, config_page, home_page, field_page, value):
    # given
    config_page.goto()

    # when
    config_page.field('String3').fill(value)
    config_page.submit_button().click()

    # then
    home_page.expect_reboot_dialogue()
    home_page.expect_at_page()

    # when
    config_page.goto()

    # then
    expect(config_page.field('String3')).to_have_value(value)


def test_form_validation_attributes(config_page):
    # when
    config_page.goto()

    # then
    expect(config_page.field('String')).not_to_have_attribute('required')
    expect(config_page.field('String')).to_have_attribute('minlength', '3')
    expect(config_page.field('String')).to_have_attribute('maxlength', '5')

    expect(config_page.field('Regex')).to_have_attribute('pattern', '[A-Z][a-z]+')
    expect(config_page.field('Regex')).to_have_attribute('required')

    expect(config_page.field('IP1')).not_to_have_attribute('pattern')
    expect(config_page.field('IP1')).not_to_have_attribute('required')

    expect(config_page.field('IP2')).not_to_have_attribute('pattern')
    expect(config_page.field('IP2')).to_have_attribute('required')

    expect(config_page.field('Password1')).not_to_have_attribute('pattern')
    expect(config_page.field('Password1')).to_have_attribute('minlength', '4')
    expect(config_page.field('Password1')).to_have_attribute('maxlength', '8')
    expect(config_page.field('Password1')).to_have_attribute('required')

    expect(config_page.field('Password2')).not_to_have_attribute('pattern')
    expect(config_page.field('Password2')).not_to_have_attribute('minlength')
    expect(config_page.field('Password2')).not_to_have_attribute('maxlength')
    expect(config_page.field('Password2')).not_to_have_attribute('required')


@pytest.mark.parametrize('label, name, value, expected_to_be_valid', [
    ('String', 'str1', 'ab', False),  # too short
    ('String', 'str1', 'hello', True),
    ('String', 'str1', '', True),  # empty is fine for non-required
    ('String', 'str1', 'hello1', False),  # too long
    ('Regex', 'str2', 'A', False),  # does not match pattern
    ('Regex', 'str2', 'ab', False),  # does not match pattern
    ('Regex', 'str2', 'Asdf', True),  # match
    ('Regex', 'str2', 'aAb', False),  # does not match pattern
    ('Regex', 'str2', 'Ab1', False),  # does not match pattern
    ('Regex', 'str2', 'hello1', False),  # too long
    ('IP1', 'ip1', '', True),  # not required, empty ok
    ('IP1', 'ip1', '127.0.0.1', True),  # correct 8-8-8-8 format
    ('IP2', 'ip2', '', False),  # required -> empty not allowed
    ('Regex', 'str2', 'hello1', False),  # too long
    ('Password1', 'p1', 'abc', False),  # too short
    ('Password1', 'p1', 'abcdefg', True),  # within 4-8
    ('Password1', 'p1', 'abcdefghi', False),  # too long
    ('Password2', 'p2', '', True),  # empty allowed
    ('Password2', 'p2', 'pw', True),  # any allowed
    ('Password2', 'p2', '12345678', True),  # any allowed
    # Double1: no constraints
    ('Double1', 'double1', '123.45', True),  # any value allowed
    ('Double1', 'double1', '', False),  # empty is not allowed
    # Double2: min=1.0, max=10.0
    ('Double2', 'double2', '5.0', True),  # within range
    ('Double2', 'double2', '0.5', False),  # below minimum
    ('Double2', 'double2', '15.0', False),  # above maximum
    # Double3: min=0.1, max=5.0, step=0.5
    ('Double3', 'double3', '2.1', True),  # within range and multiple of step
    ('Double3', 'double3', '4.1', True),  # within range and multiple of step
    ('Double3', 'double3', '0.0', False),  # below minimum
    ('Double3', 'double3', '6.0', False),  # above maximum
    # Long1: no constraints
    ('Long1', 'long1', '12345', True),  # any value allowed
    ('Long1', 'long1', '', False),  # empty is not allowed
    # Long2: min=100, max=10000
    ('Long2', 'long2', '5000', True),  # within range
    ('Long2', 'long2', '50', False),  # below minimum
    ('Long2', 'long2', '15000', False),  # above maximum
    # ULong1: no constraints
    ('ULong1', 'ulong1', '12345', True),
    ('ULong1', 'ulong1', '-1', False),  # no negative value
    ('ULong1', 'ulong1', '', False),  # empty is not allowed
    # ULong2: min=100, max=10000
    ('ULong2', 'ulong2', '5000', True),  # within range
    ('ULong2', 'ulong2', '50', False),  # below minimum
    ('ULong2', 'ulong2', '15000', False),  # above maximum
])
def test_backend_validation(home_page, config_page, default_form_dict, api_request_context,
                            label: str, name: str, value: str, expected_to_be_valid, field_page):
    form_data = default_form_dict
    form_data[name] = value
    response = api_request_context.post('/config', form=form_data, max_retries=2)

    if expected_to_be_valid:
        assert response.status == 200
    else:
        assert response.status == 400

    expected_value = value
    if name.startswith('ip') and value == '':
        expected_value = '0.0.0.0'  # hack, as IP fields are piped through IPAddress
    elif name.startswith('double') and value != '':  # hack to match default c++ formatting
        expected_value = f'{float(value):06f}'

    # check if value has actually been set
    field_page.goto()
    if expected_to_be_valid:
        expect(field_page.field_value(name)).to_have_text(expected_value)
    else:
        expect(field_page.field_value(name)).not_to_have_text(expected_value)


def test_custom_html1_visibility(config_page):
    config_page.goto()

    expect(config_page.page.get_by_text("Some visible content here")).to_be_visible()
