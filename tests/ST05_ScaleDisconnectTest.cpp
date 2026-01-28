#include <gtest/gtest.h>
#include "../src/smartscale.h"

TEST(ST05, ScaleDisconnectDuringCheckout) {
    SmartScale s;
    s.connectScale();

    ASSERT_TRUE(s.startCheckout(1.0, 10.0));
    s.disconnectScale();

    ASSERT_FALSE(s.completeTransaction());
}
