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


@pytest.fixture(scope="session", autouse=True)
def target_ip():
    with open(ROOT_DIR / '.wifi.conf', 'r') as f:
        for line in f:
            key, value = line.split('=', maxsplit=1)
            if key.startswith('WIFI_IP'):
                return value.strip()
    raise ValueError('WIFI_IP not found in .wifi.conf, please fix')


@pytest.fixture
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
