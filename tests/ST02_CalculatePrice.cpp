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
            std::vector<Product> fetchProductList() override {
                std::vector<Product> list;
                list.push_back({2,"orange", 50});
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
           bool printLabel(std::string code, std::string text) {
                return true;
           } 
    };

    class CalculatePrice : public ::testing::Test {
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

    TEST_F(CalculatePrice, CalculatePrice){
        std::string connnectionPath = "10.20.30.5";
        double weight = 90;
        double unit_price = 1.55555;
        if (smartM->initialize(connnectionPath) & smartM->isWeightStable(weight)) {
            EXPECT_NEAR(smartM->calculatePrice(weight, unit_price), 140, 0.001);
        }
    }