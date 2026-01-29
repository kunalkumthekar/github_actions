#include "smartscale.h"
#include <sstream>

SmartScale::SmartScale()
    : scaleConnected(false), serverAvailable(true),
      currentWeight(0), currentPrice(0), timeout(5) {}

bool SmartScale::connectScale() {
    scaleConnected = true;
    return true;
}

void SmartScale::disconnectScale() {
    scaleConnected = false;
}

bool SmartScale::isScaleConnected() const {
    return scaleConnected;
}

void SmartScale::weightStabilityTimeout(int timeout_) {
    if (timeout_ > timeout || !scaleConnected) return;
    Transaction t;
    t.weight = currentWeight;
    disconnectScale();
}

void SmartScale::setServerAvailable(bool state) {
    serverAvailable = state;
}

bool SmartScale::isServerAvailable() const {
    return serverAvailable;
}

bool SmartScale::startCheckout(double weight, double unitPrice) {
    if (!scaleConnected) return false;
    currentWeight = weight;
    currentPrice = unitPrice;
    return true;
}

bool SmartScale::completeTransaction() {
    if (!scaleConnected) return false;

    Transaction t;
    t.id = generateId();
    t.weight = currentWeight;
    t.unitPrice = currentPrice;
    t.synced = serverAvailable;

    localDb.push_back(t);
    return true;
}

std::vector<Transaction> SmartScale::getLocalTransactions() const {
    return localDb;
}

void SmartScale::sync() {
    if (!serverAvailable) return;
    for (auto& t : localDb) {
        t.synced = true;
    }
}

std::string SmartScale::generateId() {
    static int counter = 0;
    std::ostringstream oss;
    oss << "TX-" << ++counter;
    return oss.str();
}
