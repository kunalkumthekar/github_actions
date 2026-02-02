from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import List

@dataclass
class Transaction:
    id: int
    name: str
    unitPrice: float

@dataclass
class Product:
    id: str
    productId: int
    weight: float
    totalPrice: float
    timestamp: int

class Iscale(ABC):
    @abstractmethod
    def connect(self, connectString: str) -> bool: ...
    @abstractmethod
    def getWeight(self) -> float: ...
    @abstractmethod
    def isConnected(self) -> bool: ...

class IServer(ABC):
    @abstractmethod
    def fetchProductList(self) -> List[Product]: ...
    @abstractmethod
    def syncTransactions(self, transactions: Transaction) -> bool: ...
    @abstractmethod
    def isAvailaible(self) -> bool: ...

class IPrinter(ABC):
    @abstractmethod
    def printLabel(code: str, text: str) -> bool: ...
    