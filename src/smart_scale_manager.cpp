#include "hal.hpp"
#include "smartscale.h"


SmartScaleManager::SmartScaleManager(std::shared_ptr<IScale> s, std::shared_ptr<IServer> srv, std::shared_ptr<IPrinter> p) : 
                scale(s), server(srv), printer(p) {}

bool SmartScaleManager::initialize(std::string connectionPath) {
    /*
    Initializes the scale by confirming if connection to server is made,
    Products are fetched from the server
    */
    if (scale->connect(connectionPath)) {
        // Populating productList as it shall be needed to test
        //if tests based on false product Id fails!!
        this->productList = server->fetchProductList();
        std::cout << "DEBUG: Fetched " << productList.size() << " products." << std::endl;
        return true;
    }
    return false;
}

double SmartScaleManager::calculatePrice(double weight, double unitPrice) {
    double price = weight * unitPrice;
    return std::round(price * 100.0) / 100.0; // Round to 2 decimals
}

// CHeck if object placed on scale has stabilized.
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

    // Protecting the section of sending data to local database, and printing label
    std::lock_guard<std::mutex> lock(dbMutex);
    localDb.push_back(t);

    // Print Label
    std::cout << "DEBUG: About to call printer..." << std::endl;
    return printer->printLabel(t.id, it->name + " Total: " + std::to_string(t.totalPrice));
    // Mutex released as the scope of the methods ends
}
        
int SmartScaleManager::syncWithServer() {
    /*
    database which is populated during performCheckout method is now synced to database
    If syncTransactions is off and server is not availaible, the data shall be stored offline
    */
    if (!server->isAvailaible()) return 0;

    std::lock_guard<std::mutex> lock(dbMutex);
    int syncedCount = 0;

    // Try to sync all local transactions
    auto it = localDb.begin();
    while (it != localDb.end()) {
        // Check if server is offline and if ON :
        if (server->syncTransactions(*it)) {
            it = localDb.erase(it); // Remove from local once synced
            syncedCount++;
        } else {
            // Store data offline
            ++it;
        }
    }
    return syncedCount;
}

size_t SmartScaleManager::getLocalDbSize() {
    return localDb.size(); }