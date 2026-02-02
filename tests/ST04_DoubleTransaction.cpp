    #include <gtest/gtest.h>
    #include <string>
    #include <memory>
    #include "smartscale.h" 

    class mockScale : public IScale {
        public:
            std::string connectString = "10.20.36.2";
            bool connected_ = false;

            bool connect(std::string connectString) override {
                connected_ = true;
                return true;
            }

            int getweight() override {
                return 90;
            }

            bool isConnected() override {
                    return connected_;
                }
            };

    class mockIserver : public IServer {
        public:
            std::vector<Product> list;
            std::vector<Product> fetchProductList() override {
                // Providing repeated data to check if database record only one of
                // the duplicate and avoids the other
                list.push_back({5,"orange", 50});
                list.push_back({5,"orange", 50});
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
    class DoubleTransaction : public ::testing::Test {
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

TEST_F(DoubleTransaction, FalseTransaction) {
    std::string connectString = "10.20.30.2";
    smartM->initialize(connectString);
    std::cout << "Number of entries: " << smartM->syncWithServer() << std::endl;
    EXPECT_TRUE(smartM->performCheckout(2, 40));
    EXPECT_EQ(smartM->syncWithServer(), 1);
}