import pytest
from playwright.sync_api import Page
import dns.resolver

pytestmark = [pytest.mark.esp32]


@pytest.mark.parametrize('domain', ['example.com', 'connectivitycheck.gstatic.com'])
def test_dns_lookups_resolve_to_device_ip(target_ip: str, domain: str, network_mode: str):
    if network_mode == 'ETH':
        pytest.skip('Captive portal not available on Ethernet')
    # given
    resolver = dns.resolver.Resolver()
    resolver.nameservers = [target_ip]
    resolver.timeout = 5
    resolver.lifetime = 10

    # when
    answer = resolver.resolve(domain, 'A')

    # then
    resolved_ips = [r.address for r in answer]
    assert target_ip in resolved_ips, f"Expected {target_ip} in {resolved_ips}"


def test_non_existing_paths_return_home_page(page: Page, home_page, target_ip: str, network_mode: str):
    if network_mode == 'ETH':
        pytest.skip('Captive portal not available on Ethernet')
    # when
    page.goto(f'http://{target_ip}/non-existing-path')

    # then
    home_page.expect_at_page()
