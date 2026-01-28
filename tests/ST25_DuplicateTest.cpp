TEST(ST25, PreventDuplicateSync) {
    SmartScale s;
    s.connectScale();
    s.setServerAvailable(false);

    s.startCheckout(1.0, 3.0);
    s.completeTransaction();

    s.setServerAvailable(true);
    s.sync();
    s.sync(); // second sync

    auto tx = s.getLocalTransactions();
    ASSERT_EQ(tx.size(), 1);
}
