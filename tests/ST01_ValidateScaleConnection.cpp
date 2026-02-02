#include <gtest/gtest.h>
#include <string>
#include <memory>
#include "smartscale.h" 

class MockScale : public IScale {
public:
    bool connect(std::string path) override { return true; }
    int getweight() override { return 25; }
    bool isConnected() override { return true; }
};

class MockServer : public IServer {
public:
    std::vector<Product> fetchProductList() override {
        // FIX: You must push a product or 'at(0)' will crash!
        std::vector<Product> list;
        list.push_back({1, "Apple", 100.0});
        return list;
    }
    bool syncTransactions(const hal::Transaction &t) override { return true; }
    bool isAvailaible() override { return true; }
};

class MockPrinter : public IPrinter {
public:
    bool printLabel(std::string code, std::string txt) override { return true; }
};

// Setting up the fixture for Gtest

class ValidateScaleConnection : public ::testing::Test {
protected:
    // 2. Use shared_ptr for the mocks so they can be passed to the SUT
    std::shared_ptr<MockScale> s;
    std::shared_ptr<MockServer> srv;
    std::shared_ptr<MockPrinter> p;
    std::unique_ptr<SmartScaleManager> sManager;

    void SetUp() override {
        s = std::make_shared<MockScale>();
        srv = std::make_shared<MockServer>();
        p = std::make_shared<MockPrinter>();

        // 3. Dependency Injection: Pass the actual instances
        sManager = std::make_unique<SmartScaleManager>(s, srv, p);
    }
};

TEST_F(ValidateScaleConnection, Validation) {
    // Now sManager is safely initialized with valid pointers
    bool result = sManager->initialize("10.20.2.60");
    EXPECT_TRUE(result);
    
    double weight = 2.5;
    double unit_price = 2;
    EXPECT_EQ(sManager->calculatePrice(weight, unit_price), 5.0);

}