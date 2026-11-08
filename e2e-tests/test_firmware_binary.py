import pathlib
import warnings

import pytest

pytestmark = [pytest.mark.offline]

ROOT_DIR = pathlib.Path(__file__).parent.parent.absolute()

# (pio env, upper size limit). Fail if a build grows beyond its limit, warn if it shrinks.
FIRMWARES = [
    ('test-e2e-esp32', 1066640),
    ('example-basic-wifi', 1052704),
    ('example-minimal', 1008880),
    ('example-minimal-ethernet', 530992),
    ('example-empty-baseline', 922768),
]


@pytest.mark.parametrize('env, max_size', FIRMWARES, ids=[env for env, _ in FIRMWARES])
def test_firmware_size(env, max_size):
    bin_path = ROOT_DIR / '.pio/build' / env / 'firmware.bin'
    if not bin_path.exists():
        pytest.fail(f'{bin_path} not found - build it first (scripts/test offline)')

    size = bin_path.stat().st_size
    if size > max_size:
        pytest.fail(f'{env} firmware too big: {size} > {max_size} bytes')
    if size < max_size:
        warnings.warn(f'{env} firmware smaller than expected: {size} < {max_size} bytes')
