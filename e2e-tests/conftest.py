import os
from typing import Generator

import pytest
import pathlib

from playwright.sync_api import Page, Playwright, APIRequestContext, expect, BrowserContext

ROOT_DIR = pathlib.Path(__file__).parent.parent.absolute()

DEVICE_NAME = 'e2e-testing'
TIMEOUT = 60_000

from page_models import HomePage, ConfigurationPage, FieldPage

expect.set_options(timeout=TIMEOUT)


# override default navigation timeout
@pytest.fixture
def page(context: BrowserContext) -> Generator[Page, None, None]:
    page = context.new_page()
    page.set_default_navigation_timeout(TIMEOUT)
    yield page


@pytest.fixture(scope="session", autouse=True)
def base_url(target_ip):
    return f'http://{target_ip}'


def _read_network_conf() -> dict[str, str]:
    config = {}
    with open(ROOT_DIR / '.network.conf', 'r') as f:
        for line in f:
            key, value = line.split('=', maxsplit=1)
            config[key.strip()] = value.strip()
    return config


def _get_network_modes() -> list[str]:
    return os.environ.get('NETWORK_MODES', 'WIFI').split(',')


@pytest.fixture(scope="session", autouse=True, params=_get_network_modes())
def target_ip(request: pytest.FixtureRequest):
    network_mode = request.param
    config = _read_network_conf()
    ip_key = f'{network_mode}_ip'.upper()
    if ip_key not in config:
        raise ValueError(f'{ip_key} not found in .network.conf, please fix')
    return config[ip_key]


@pytest.fixture(scope="session")
def network_mode(target_ip: str) -> str:
    config = _read_network_conf()
    for mode in _get_network_modes():
        if config.get(f'{mode}_ip'.upper()) == target_ip:
            return mode
    raise ValueError(f'Network mode not found for IP {target_ip}')


@pytest.fixture(scope='session')
def device_name():
    return 'e2e-testing'


@pytest.fixture
def home_page(page: Page, device_name):
    return HomePage(page, device_name)


@pytest.fixture
def config_page(page: Page, device_name):
    return ConfigurationPage(page, device_name)


@pytest.fixture
def field_page(page: Page, device_name):
    return FieldPage(page, device_name)


@pytest.fixture(scope="session")
def api_request_context(playwright: Playwright, base_url: str) -> Generator[APIRequestContext, None, None]:
    request_context = playwright.request.new_context(base_url=base_url)
    yield request_context
    request_context.dispose()


@pytest.fixture(autouse=True)
def wait_for_device(api_request_context: APIRequestContext):
    for _ in range(200):
        try:
            api_request_context.get('/', timeout=100)
            break
        except Exception:
            pass
    else:
        api_request_context.get('/', timeout=100)
    yield
