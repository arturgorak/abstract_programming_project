#include "lib/googletest/include/gtest/gtest.h"

#include "../battlefield/Battlefield.h"


class WombatFixture : public ::testing::Test {
protected:
    WomKnight<int>* knight;
    WoArcher<int>* archer;
    Battlefield<int> battlefield;

    void SetUp() override {
        knight = new WomKnight<int>("TestKnight", new Falchion<int>(), new HavelArmor<int>(), battlefield);
        archer = new WoArcher<int>("TestArcher", new Bow<int>(), new ThiefArmor<int>(), battlefield);

    }

    void TearDown() override {
        delete knight;
        delete archer;
    }
};

TEST_F(WombatFixture, ReduceDistance) {
    EXPECT_EQ(battlefield.get_current_distance(), 10);

    knight->reduce_distance();
    EXPECT_EQ(battlefield.get_current_distance(), 2);

    knight->reduce_distance();
    EXPECT_EQ(battlefield.get_current_distance(), 0);
}


TEST_F(WombatFixture, IncreaseDistance) {
    EXPECT_EQ(battlefield.get_current_distance(), 10);

    archer->increase_distance();
    EXPECT_EQ(battlefield.get_current_distance(), 22);

    archer->increase_distance();
    EXPECT_EQ(battlefield.get_current_distance(), 30);
}

TEST_F(WombatFixture, Wait) {
    EXPECT_EQ(archer->current_stamina, 5);

    archer->wait();
    EXPECT_EQ(archer->stamina, 7);

    archer->wait();
    EXPECT_EQ(archer->stamina, 7);
}

TEST_F(WombatFixture, IsDead) {
    EXPECT_EQ(archer->is_dead(), false);

    archer->health = 0;
    EXPECT_EQ(archer->is_dead(), true);

}

