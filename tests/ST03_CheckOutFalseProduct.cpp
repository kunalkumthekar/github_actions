#include <gtest/gtest.h>
#include <string>
#include <memory>
#include "smartscale.h" 

class mockScale : public IScale {
    public:
        std::string connectString = "10.20.36.2";

        bool connect(std::string connectString) override {
            return true;
        }

        int getweight() override {
            return 90;
        }

        bool isConnected() override {
            if(connect(connectString)) {
                return true;
            } 
            else {return false;}
        }
    };

class mockIserver : public IServer {
    public:
        std::vector<Product> list;
        std::vector<Product> fetchProductList() override {
            list = {
            {1, "apple", 100},
            {2, "orange", 250},
            {3, "banana", 80}
    };
            return list;
        }
        bool syncTransactions(const hal::Transaction &transaction) override {
            return true;
        }
        bool isAvailaible() override {
            return true;
        }
};
class mockPrinter : public IPrinter {
    public:
       bool printLabel(std::string code, std::string text) override {
            return true;
       } 
};
class CheckoutFalseProduct : public ::testing::Test {
        /*
        Fixture class which shall instantiate the main class by calling mock classes in it
        */
    protected:
        std::shared_ptr<mockScale> scale;
        std::shared_ptr<mockIserver> server;
        std::shared_ptr<mockPrinter> printer;
        std::unique_ptr<SmartScaleManager> smartM;
        void SetUp() override {
            scale = std::make_shared<mockScale>();
            server = std::make_shared<mockIserver>();
            printer = std::make_shared<mockPrinter>();
            smartM = std::make_unique<SmartScaleManager>(scale, server, printer);
        }
        void TearDown() override {
        }
};

TEST_F(CheckoutFalseProduct, FalseObject) {
    std::string connectionPath = "192.168.2.60";
    double weight = 1000;

    smartM->initialize(connectionPath);

    EXPECT_TRUE(smartM->performCheckout(2, weight));
}

