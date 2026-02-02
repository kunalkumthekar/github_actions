def test_valid_checkout(smartscale):
    assert smartscale.perform_checkout(1, 2.0)
    assert smartscale.get_local_db_size() == 1
    assert smartscale.calculatePrice(25,2) == 50