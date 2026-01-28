#pragma once
#include <string>
#include <vector>

struct Transaction {
    std::string id;
    double weight;
    double unitPrice;
    bool synced;
};

class SmartScale {
public:
    SmartScale();

    bool connectScale();
    void disconnectScale();
    bool isScaleConnected() const;

    void setServerAvailable(bool state);
    bool isServerAvailable() const;

    bool startCheckout(double weight, double unitPrice);
    bool completeTransaction();

    std::vector<Transaction> getLocalTransactions() const;
    void sync();

private:
    bool scaleConnected;
    bool serverAvailable;
    double currentWeight;
    double currentPrice;
    std::vector<Transaction> localDb;

    std::string generateId();
};
