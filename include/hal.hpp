#ifndef HAL
#define HAL

#include <iostream>
#include <string>
#include <vector>

// Data Struct for the product
struct Product {
    int id;
    std::string name;
    double unit_price;
};

// Data structure for completed transaction
namespace hal{
    struct Transaction {
    std::string id;
    int productId;
    double weight;
    double totalPrice;
    long timestamp;
};
}

// Interfaces for mocking Hardware

class IScale {
    /*
    Weighing Scale interface: Shall give confirmation of
    successfull connection, weight placed on the scale
    */
    public:
        virtual ~IScale() = default;
        virtual bool connect(std::string connectString) = 0;
        virtual int getweight() = 0;
        virtual bool isConnected() = 0;
};

class IServer {
    /*
    Server Interface: Shall fetch list of accepted products,
    sync transaction with server after server downtime
    */
    public:
        virtual ~IServer() = default;
        virtual std::vector<Product> fetchProductList() = 0;
        virtual bool syncTransactions(const hal::Transaction &transaction) = 0;
        virtual bool isAvailaible() = 0;
};

class IPrinter {
    /*
    Shall print label after inputting barcode and text data
    */
    public:
        virtual ~IPrinter() = default;
        virtual bool printLabel(std::string barcode, std::string text) = 0;
};

#endif