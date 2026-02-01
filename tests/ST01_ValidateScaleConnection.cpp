#include <gtest/gtest.h>
#include <string>
#include "smartscale.h"

std::shared_ptr<IScale> scale;
std::shared_ptr<IServer> server;
std::shared_ptr<IPrinter> printer;

Product prod;

class MockScale : public IScale {
    public:
        std::string connection_path = "10.20.2.60";
        int weight = 25;
        bool isConnected_ = true;

        bool connect(std::string connection_path) override {
            return true;
        }

        int getweight() override {
            return weight;
        }

        bool isConnected() override{
            return isConnected_;
        }
};

class MockServer : public IServer {
    public:
        std::vector<Product> prod;
        hal::Transaction trans;
        int weight = 25;
        bool isConnected_ = true;

        std::vector<Product> fetchProductList() override {
            prod.at(0).id = 1;
            prod.at(0).name = "Apple";
            prod.at(0).unit_price = 100;
            return prod;
        }

        bool syncTransactions(const hal::Transaction &lastTransaction) override {
            this->trans = lastTransaction;
            return true;
        }

        bool isAvailaible() override{
            return isConnected_;
        }
};

class MockPrinter : public IPrinter {
    public:
        std::string barcode = "927588298";
        std::string text = "Apples";

        bool printLabel(std::string code, std::string txt) override {
            this->barcode = code;
            this->text = txt;
            return true;
        };
};

// TODO: mock up printer and then plug it in the Test fixture

class ValidateScaleConnection : public ::testing::Test {
    protected:
        MockScale s;
        MockServer srv;
        MockPrinter p;

        void SetUp() override {
            EXPECT_TRUE(s.isConnected());
        }
        void TearDown() override {
        }
};


TEST(ValidateScaleConnection, Validation) {
    SmartScaleManager sManager(scale, server, printer);

    // Connection path

    prod.id = 1;
    prod.name = "Apple";
    prod.unit_price = 20;

    
    
}
