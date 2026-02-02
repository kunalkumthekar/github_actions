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
    /*
    Shall accept shared pointers of object of scale, server, and printer
    interfaces. Shared pointers shall make sure the communication between
    Mock classes, interface classes and SmartScaleManager stays unified.
    i.e Data flow is now predictable.

    Boss class which shall process mock data
    */
    private:
    // Interface pointers 
        std::shared_ptr<IScale> scale;
        std::shared_ptr<IServer> server;
        std::shared_ptr<IPrinter> printer;

        std::vector<Product> productList;
        std::vector<hal::Transaction> localDb;
        
        // Mutex to gaurd sending database over server
        std::mutex dbMutex;
        // Place holder to calculate weight difference in two weight readings
        double lastWeight = 0.0;
        std::chrono::steady_clock::time_point lastChangeTime;

        public:
        // Constructor taking shared pointers of interfaces as arguments
            SmartScaleManager(std::shared_ptr<IScale> s, std::shared_ptr<IServer> srv, std::shared_ptr<IPrinter> p);

            bool initialize(std::string connectionPath);

            double calculatePrice(double weight, double unitPrice);

            bool isWeightStable(double currentWeight);

            bool performCheckout(int productId, double weight);
        
            int syncWithServer();

            size_t getLocalDbSize();
};  
