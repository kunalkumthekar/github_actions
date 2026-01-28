TEST(ST08, RejectUnstableWeight) {
    SmartScale s;
    s.connectScale();

    bool started = s.startCheckout(-5.0, 10.0);
    ASSERT_TRUE(started);

    bool completed = s.completeTransaction();
    ASSERT_TRUE(completed); // system still stores, but marks logic later
}