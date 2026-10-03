import pytest
from playwright.sync_api import Page, expect


def test_home_page_renders(home_page):
    # when
    home_page.goto()

    # then
    home_page.expect_at_page()
    expect(home_page.page.locator('p')).to_contain_text('This is custom page html')


def test_reboot(home_page):
    # given
    home_page.goto()

    # when
    home_page.reboot_button().click()

    # then
    home_page.expect_reboot_dialogue()

    # then
    home_page.expect_at_page()
