import pytest
from playwright.sync_api import expect

from page_models import HomePage

pytestmark = [pytest.mark.esp32]


@pytest.fixture(scope="module")
def clean_config_without_password(browser, base_url):
    context = browser.new_context()
    page = context.new_page()
    page.set_default_navigation_timeout(60_000)
    page.goto(base_url)
    home_page = HomePage(page, 'e2e-testing')
    home_page.reset()
    yield home_page
    context.close()


@pytest.mark.parametrize('method,path,expected_status', [
    ('get', '/config', 200),
    ('post', '/config', 400),  # we send an empty body, just for testing
    ('get', '/update', 200),
    # ('post', '/update', 200), # this does not seem to work properly due to connection resets
    # ('post', '/reset', 200),  # reset is not a good idea, as it removes the password and therefore destroys the test
    ('post', '/reboot', 200),
])
def test_all_endpoints_are_reachable(home_page, api_request_context, method, path,
                                     expected_status):
    # the regular api request context is not logged in
    home_page.goto()
    home_page.expect_at_page()  # ensure device is reachable

    if method == 'get':
        response = api_request_context.get(path)
    elif method == 'post':
        response = api_request_context.post(path)
    else:
        raise ValueError('Wrong test setup')

    assert response.status == expected_status, f"Expected {expected_status} for {method} {path}, got {response.status}"


def test_home_page_shows_regular_menu(home_page):
    # when
    home_page.goto()
    home_page.expect_at_page()

    # then
    expect(home_page.login_button()).not_to_be_visible()
    expect(home_page.configuration_button()).to_be_visible()
    expect(home_page.ota_button()).to_be_visible()
    expect(home_page.reset_button()).to_be_visible()
    expect(home_page.reboot_button()).to_be_visible()
