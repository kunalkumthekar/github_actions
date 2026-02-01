#pragma once
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <cmath>
#include <algorithm>
#include <chrono>
#include "hal.hpp"

class SmartScaleManager {
    private:
        std::shared_ptr<IScale> scale;
        std::shared_ptr<IServer> server;
        std::shared_ptr<IPrinter> printer;

        std::vector<Product> productList;
        std::vector<hal::Transaction> localDb;
        // Mutex to gaurd sending data over server
        std::mutex dbMutex;
        double lastWeight = 0.0;
        std::chrono::steady_clock::time_point lastChangeTime;

        public:
        // Constructor
            SmartScaleManager(std::shared_ptr<IScale> s, std::shared_ptr<IServer> srv, std::shared_ptr<IPrinter> p);
            
            bool initialize(std::string connectionPath);

            double calculatePrice(double weight, double unitPrice);

            bool isWeightStable(double currentWeight);

            bool performCheckout(int productId, double weight);
        
            int syncWithServer();

        // Helper for testing
            size_t getLocalDbSize();
};  
