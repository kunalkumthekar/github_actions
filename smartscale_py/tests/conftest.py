import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
from app.smartscale import SmartScaleManager
from app.hal import Product
import pytest

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

    def fetchProductList(self):
        return [Product("Cheese", 1, 10.0, 100.0, 123)]

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
