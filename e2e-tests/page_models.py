import re
import time

from playwright.sync_api import Locator, Error
from playwright.sync_api import Page, expect

RETRY_DELAY_S = 2


class BasePage:
    def __init__(self, page: Page, path: str, device_name: str, subtitle: str | None = None):
        self.page = page
        self.path = path
        self.device_name = device_name
        self.subtitle = subtitle

    def _title(self):
        if self.subtitle:
            return self.device_name + ' — ' + self.subtitle
        else:
            return self.device_name

    def goto(self):
        i = 0
        while True:
            try:
                return self.page.goto(self.path, timeout=5_000)
            except Error:
                i += 1
                if i >= 12:
                    raise
                time.sleep(RETRY_DELAY_S)

    def expect_at_page(self):
        expect(self.page).to_have_url(re.compile(f'^http://[0-9.]+{self.path}[?]?$'))
        expect(self.page).to_have_title(self._title())
        expect(self.page.locator('h1')).to_contain_text(self.device_name)
        if self.subtitle:
            expect(self.page.locator('h2')).to_contain_text(self.subtitle)

    def back_button(self):
        return self.page.get_by_text('Back')

    def expect_reboot_dialogue(self):
        expect(self.page).to_have_title(self.device_name)
        expect(self.page.get_by_text('Rebooting')).to_be_visible()


class HomePage(BasePage):
    def __init__(self, page: Page, device_name: str):
        super().__init__(page, '/', device_name)

    def configuration_button(self):
        return self.page.get_by_text('Configuration', exact=True)

    def ota_button(self):
        return self.page.get_by_text('OTA update')

    def reboot_button(self):
        return self.page.get_by_text('Reboot', exact=True)

    def reset_button(self):
        return self.page.get_by_text('Reset configuration')

    def login_button(self):
        return self.page.get_by_text('Login', exact=True)

    def ensure_login(self):
        if self.login_button().count() > 0:
            self.login_button().click()

    def reset(self):
        print("Resetting ...")
        self.goto()
        # noinspection bad-argument-type
        self.page.once('dialog', lambda dialog: dialog.accept())
        self.reset_button().click()
        self.expect_reboot_dialogue()
        self.expect_at_page()

class ConfigurationPage(BasePage):
    def __init__(self, page: Page, device_name: str):
        super().__init__(page, '/config', device_name, subtitle='Configuration')

    def field(self, label: str):
        return self.page.get_by_label(label, exact=True)

    def submit_button(self):
        return self.page.get_by_text('Save')


class FieldPage(BasePage):
    def __init__(self, page: Page, device_name: str):
        super().__init__(page, '/fields', device_name, subtitle='Fields')

    def field_value(self, key: str) -> Locator:
        return self.page.locator(f'xpath=//tr[td[1]="{key}"]/td[2]')


class OtaPage(BasePage):
    def __init__(self, page: Page, device_name: str):
        super().__init__(page, '/update', device_name, subtitle='OTA update')

    def file_input(self):
        return self.page.locator('input[type="file"]')

    def upload_button(self):
        return self.page.get_by_text('Upload')

    def upload_firmware(self, firmware_path: str):
        self.goto()
        self.file_input().set_input_files(firmware_path)
        # handle the confirm dialog
        self.page.once('dialog', lambda dialog: dialog.accept())
        self.upload_button().click()
