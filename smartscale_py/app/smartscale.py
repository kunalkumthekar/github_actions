from app.hal import Iscale, IPrinter, IServer, Transaction, Product
from typing import List
import time

class SmartScaleManager:
    def __init__(self, scale:Iscale, server:IServer, printer:IPrinter):
        self.scale = scale
        self.printer = printer
        self.server = server
        self.productList: List[Product] = []
        self.transaction: List[Transaction] = []
        self.lastWeight=0.0
    
    def initialize(self, connectionPath: str) -> bool:
        if self.scale.connect(connectionPath):
            self.productList = self.server.fetch_product_list()
            return True
        else:
            return False
        
    def calculatePrice(self, weight: float, unitPrice: float) -> float:
        return round(weight * unitPrice)

    def isWeightStable(self, currentWeight: float) -> bool:
        if (abs(currentWeight - self.lastWeight))< 0.0002:
            return True
        self.lastWeight = currentWeight
        return False
    
    def perform_checkout(self, product_id, weight):
        if not self.scale.is_connected():
            return False
        if weight < 0:
            return False

        product = next((p for p in self.product_list if p.id == product_id), None)
        if not product:
            return False

        tx = Transaction(
            id=f"TXN-{int(time.time()*1000)}",
            productId=product_id,
            weight=weight,
            totalPrice=self.calculate_price(weight, product.unit_price),
            timestamp=int(time.time())
        )

        with self.db_lock:
            self.local_db.append(tx)

        return self.printer.print_label(tx.id, f"{product.name} Total: {tx.totalPrice}")

    def sync_with_server(self):
        if not self.server.is_available():
            return 0

        synced = 0
        with self.db_lock:
            remaining = []
            for tx in self.local_db:
                if self.server.sync_transaction(tx):
                    synced += 1
                else:
                    remaining.append(tx)
            self.local_db = remaining
        return synced

    def get_local_db_size(self):
        return len(self.local_db)

