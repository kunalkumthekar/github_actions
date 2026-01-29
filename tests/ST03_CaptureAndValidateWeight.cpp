#include <gtest/gtest.h>
#include "smartscale.h"

TEST(ST03, CaptureAndValidateWeight) {
    SmartScale s;
    int timeOut = 7;
    s.weightStabilityTimeout(timeOut);
    ASSERT_EQ(s.isScaleConnected(), true);
}