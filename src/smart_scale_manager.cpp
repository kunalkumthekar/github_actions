#include "hal.hpp"
#include "smartscale.h"


SmartScaleManager::SmartScaleManager(std::shared_ptr<IScale> s, std::shared_ptr<IServer> srv, std::shared_ptr<IPrinter> p) : 
                scale(s), server(srv), printer(p) {}

bool SmartScaleManager::initialize(std::string connectionPath) {
    if (scale->connect(connectionPath)) {
        server->fetchProductList();
        return true;
    }
    return false;
}

double SmartScaleManager::calculatePrice(double weight, double unitPrice) {
    double price = weight * unitPrice;
    return std::round(price * 100.0) / 100.0; // Round to 2 decimals
}

bool SmartScaleManager::isWeightStable(double currentWeight) {
    if (std::abs(currentWeight - lastWeight) <= 0.002) {
        return true; 
    }
    lastWeight = currentWeight;
    return false;
    }

bool SmartScaleManager::performCheckout(int productId, double weight) {
    /*
    Checks if weight is valid, and then populates the product information 
    from Product struct in the Transaction struct, and then sends the 
    information over from the server to the database under mutex protection.
    Lastly prints the label.
    */
    if (!scale->isConnected()) return false;
    if (weight < 0) return false;
    
    // Search for given product in the list of products
    auto it = std::find_if(productList.begin(), productList.end(),
                            [&](const Product &p) {return p.id == productId;});
    if (it == productList.end()) return false;
    // Populating Transaction struct
    hal::Transaction t;
    t.id = "TXN-" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    t.productId = productId;
    t.weight = weight;
    t.totalPrice = calculatePrice(weight, it->unit_price);
    t.timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::lock_guard<std::mutex> lock(dbMutex);
    localDb.push_back(t);

    // Print Label
    return printer->printLabel(t.id, it->name + " Total: " + std::to_string(t.totalPrice));
}
        
int SmartScaleManager::syncWithServer() {
    if (!server->isAvailaible()) return 0;

    std::lock_guard<std::mutex> lock(dbMutex);
    int syncedCount = 0;

    // Try to sync all local transactions
    auto it = localDb.begin();
    while (it != localDb.end()) {
        if (server->syncTransactions(*it)) {
            it = localDb.erase(it); // Remove from local once synced
            syncedCount++;
        } else {
            ++it;
        }
    }
    return syncedCount;
}

    size_t SmartScaleManager::getLocalDbSize() {
        return localDb.size(); }