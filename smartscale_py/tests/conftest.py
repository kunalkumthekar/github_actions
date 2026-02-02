import pytest
from app.smartscale import SmartScaleManager
from app.hal import Product


class MockScale:
    def __init__(self):
        self.connected = False

    def connect(self, path):
        self.connected = True
        return True

    def get_weight(self):
        return 1.0

    def is_connected(self):
        return self.connected


class MockServer:
    def __init__(self, available=True):
        self.available = available
        self.synced = []

    def fetch_product_list(self):
        return [Product(1, "Cheese", 10.0)]

    def sync_transaction(self, tx):
        if not self.available:
            return False
        self.synced.append(tx)
        return True

    def is_available(self):
        return self.available


class MockPrinter:
    def print_label(self, barcode, text):
        return True


@pytest.fixture
def smartscale():
    scale = MockScale()
    server = MockServer()
    printer = MockPrinter()
    mgr = SmartScaleManager(scale, server, printer)
    mgr.initialize("/dev/scale")
    return mgr
