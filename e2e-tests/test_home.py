import pytest
from playwright.sync_api import Page, expect

pytestmark = [pytest.mark.esp32]


def test_home_page_renders(home_page):
    # when
    home_page.goto()

    # then
    home_page.expect_at_page()
    expect(home_page.page.get_by_text('This is custom page html')).to_be_visible()
    expect(home_page.page.locator('.footer')).to_contain_text('version 1.2.3')
    expect(home_page.page.locator('.footer a')).to_have_attribute('href', 'https://github.com/tom-mi/simple-esp-configurator')


def test_footer_styling(home_page):
    # when
    home_page.goto()

    # then
    footer = home_page.page.locator('.footer')
    expect(footer).to_have_css('text-align', 'center')
    expect(footer).to_have_css('font-size', '11.2px')  # 70% of default 16px
    expect(footer).to_have_css('color', 'rgb(102, 102, 102)')


def test_reboot(home_page):
    # given
    home_page.goto()

    # when
    home_page.reboot_button().click()

    # then
    home_page.expect_reboot_dialogue()

    # then
    home_page.expect_at_page()
