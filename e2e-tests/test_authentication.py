import pytest
from playwright.sync_api import expect, Playwright

from page_models import HomePage, ConfigurationPage

pytestmark = [pytest.mark.esp32]

ADMIN_PASSWORD = "testpw123"


@pytest.fixture
def logged_in_page(browser, base_url, device_name):
    context = browser.new_context(
        base_url=base_url,
        # in case we need the password, send it always to make the test setup stable
        http_credentials={"username": 'admin', "password": ADMIN_PASSWORD, "send": "always"},
    )
    page = context.new_page()
    page.set_default_navigation_timeout(60_000)
    home_page = HomePage(page, device_name=device_name)
    home_page.goto()
    home_page.expect_at_page()
    home_page.ensure_login()

    yield page

    context.close()


@pytest.fixture
def logged_in_home_page(logged_in_page, device_name):
    yield HomePage(logged_in_page, device_name=device_name)


@pytest.fixture
def logged_in_api_request_context(playwright: Playwright, base_url, device_name):
    request_context = playwright.request.new_context(
        base_url=base_url,
        # in case we need the password, send it always to make the test setup stable
        http_credentials={"username": 'admin', "password": ADMIN_PASSWORD, "send": "always"},
    )

    yield request_context

    request_context.dispose()


@pytest.fixture(scope="module", autouse=True)
def clean_config_with_password(browser, base_url):
    context = browser.new_context(
        base_url=base_url,
        # in case we need the password, send it always to make the test setup stable
        http_credentials={"username": 'admin', "password": ADMIN_PASSWORD, "send": "always"},
    )
    page = context.new_page()
    page.set_default_navigation_timeout(60_000)

    home_page = HomePage(page, 'e2e-testing')
    config_page = ConfigurationPage(page, 'e2e-testing')

    # Reset config first
    home_page.goto()
    home_page.expect_at_page()
    home_page.ensure_login()
    home_page.reset()

    # Set password
    config_page.goto()
    config_page.field('Admin Password').fill(ADMIN_PASSWORD)
    config_page.submit_button().click()
    home_page.expect_reboot_dialogue()
    home_page.expect_at_page()

    yield home_page

    # Reset config again after tests to clear the password
    home_page.goto()
    home_page.expect_at_page()
    home_page.ensure_login()
    home_page.reset()

    context.close()


@pytest.mark.parametrize('method,path', [
    ('get', '/config'),
    ('post', '/config'),
    ('get', '/update'),
    ('post', '/update'),
    ('post', '/reset'),
    ('post', '/reboot'),
])
def test_when_logged_out_protected_endpoints_return_401(api_request_context, method, path):
    # the regular api request context is not logged in
    if method == 'get':
        response = api_request_context.get(path)
    elif method == 'post':
        response = api_request_context.post(path)
    else:
        raise ValueError('Wrong test setup')

    assert response.status == 401, f"Expected 401 for {method} {path}, got {response.status}"


@pytest.mark.parametrize('method,path,expected_status', [
    ('get', '/config', 200),
    ('post', '/config', 400),  # we send an empty body, just for testing
    ('get', '/update', 200),
    # ('post', '/update', 200), # this does not seem to work properly due to connection resets
    # ('post', '/reset', 200),  # reset is not a good idea, as it removes the password and therefore destroys the test
    ('post', '/reboot', 200),
])
def test_when_logged_in_protected_endpoints_are_reachable(home_page, logged_in_api_request_context, method, path,
                                                          expected_status):
    # the regular api request context is not logged in
    home_page.goto()
    home_page.expect_at_page()  # ensure device is reachable

    if method == 'get':
        response = logged_in_api_request_context.get(path)
    elif method == 'post':
        response = logged_in_api_request_context.post(path)
    else:
        raise ValueError('Wrong test setup')

    assert response.status == expected_status, f"Expected {expected_status} for {method} {path}, got {response.status}"


def test_when_logged_out_home_page_shows_login_only(home_page):
    # the regular "home_page" fixture is not logged in

    # when
    home_page.goto()
    home_page.expect_at_page()

    # then
    expect(home_page.login_button()).to_be_visible()
    expect(home_page.configuration_button()).not_to_be_visible()
    expect(home_page.ota_button()).not_to_be_visible()
    expect(home_page.reset_button()).not_to_be_visible()
    expect(home_page.reboot_button()).not_to_be_visible()


def test_when_logged_in_home_page_shows_regular_menu(logged_in_home_page):
    # when
    logged_in_home_page.goto()
    logged_in_home_page.expect_at_page()

    # then
    expect(logged_in_home_page.login_button()).not_to_be_visible()
    expect(logged_in_home_page.configuration_button()).to_be_visible()
    expect(logged_in_home_page.ota_button()).to_be_visible()
    expect(logged_in_home_page.reset_button()).to_be_visible()
    expect(logged_in_home_page.reboot_button()).to_be_visible()
