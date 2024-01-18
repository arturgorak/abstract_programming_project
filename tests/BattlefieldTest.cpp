#include "lib/googletest/include/gtest/gtest.h"

#include "../battlefield/Battlefield.h"



class BattlefieldFixture : public ::testing::Test {
protected:
    Battlefield<int>* battlefield;

    void SetUp() override {
        battlefield = new Battlefield<int>();
    }

    void TearDown() override {
        delete battlefield;
    }
};

TEST_F(BattlefieldFixture, ReduceDistance) {
    battlefield->reduce_distance(5);
    EXPECT_EQ(battlefield->get_current_distance(), 5);

    battlefield->reduce_distance(3);
    EXPECT_EQ(battlefield->get_current_distance(), 2);

    battlefield->reduce_distance(3);
    EXPECT_EQ(battlefield->get_current_distance(), 0);
}

TEST_F(BattlefieldFixture, IncreaseDistance) {
    battlefield->increase_distance(5);
    EXPECT_EQ(battlefield->get_current_distance(), 15);

    battlefield->increase_distance(10);
    EXPECT_EQ(battlefield->get_current_distance(), 25);

    battlefield->increase_distance(10);
    EXPECT_EQ(battlefield->get_current_distance(), 30);
}
