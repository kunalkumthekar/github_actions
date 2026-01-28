TEST(ST20, PrinterDisconnectedSimulation) {
    SmartScale s;
    s.connectScale();
    s.setServerAvailable(false);

    s.startCheckout(2.0, 5.0);
    ASSERT_TRUE(s.completeTransaction());

    auto tx = s.getLocalTransactions();
    ASSERT_EQ(tx[0].synced, false);
}