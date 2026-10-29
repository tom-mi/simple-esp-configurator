import pathlib
import subprocess
from typing import Generator

import pytest
from playwright.sync_api import Page

from page_models import HomePage, OtaPage

pytestmark = [pytest.mark.esp32]

ROOT_DIR = pathlib.Path(__file__).parent.parent.absolute()


@pytest.fixture(scope="module")
def default_firmware_path() -> pathlib.Path:
    return ROOT_DIR / '.pio' / 'build' / 'test-e2e-esp32' / 'firmware.bin'


@pytest.fixture(scope="module")
def ota_firmware_path() -> pathlib.Path:
    return ROOT_DIR / '.pio' / 'build' / 'test-e2e-ota-esp32' / 'firmware.bin'


@pytest.fixture
def restore_firmware_after_test(default_firmware_path: pathlib.Path) -> Generator[None, None, None]:
    """Restore the original firmware after the test."""
    yield
    subprocess.check_call(
        ['pio', 'run', '--environment', 'test-e2e-esp32', '--target', 'upload'],
        cwd=ROOT_DIR,
    )


def test_ota_update(default_firmware_path: pathlib.Path, ota_firmware_path: pathlib.Path,
                    page: Page, restore_firmware_after_test):
    # given
    assert default_firmware_path.exists(), f"Default firmware not found: {default_firmware_path}"
    assert ota_firmware_path.exists(), f"OTA firmware not found: {ota_firmware_path}"

    # when: flash OTA firmware via web interface
    ota_page = OtaPage(page, 'e2e-testing')
    ota_page.upload_firmware(str(ota_firmware_path))

    # then: verify OTA firmware is present
    home_page = HomePage(page, 'e2e-testing-ota')
    home_page.goto()
    home_page.expect_at_page()
