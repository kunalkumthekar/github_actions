#include <gtest/gtest.h>
#include "smartscale.h"

TEST(ST24, SyncAfterReconnect) {
    SmartScale s;
    s.connectScale();
    s.setServerAvailable(false);

    s.startCheckout(1.0, 3.0);
    s.completeTransaction();

    s.setServerAvailable(true);
    s.sync();

    auto tx = s.getLocalTransactions();
    ASSERT_TRUE(tx[0].synced);
}