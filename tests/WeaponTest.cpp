#include "lib/googletest/include/gtest/gtest.h"

#include "../battlefield/Battlefield.h"


class WeaponFixture : public ::testing::Test {
protected:
    Battlefield<int> battlefield;
    WoArcher<int>* attacker;
    WomKnight<int>* defender;
    Bow<int>* weapon;

    void SetUp() override {
          weapon = new Bow<int>();
          attacker = new WoArcher<int>("TestWombat", weapon, new ThiefArmor<int>(), battlefield);
          defender = new WomKnight<int>("TestWombat", new Falchion<int>(), new HavelArmor<int>(), battlefield);

    }

    void TearDown() override {
        delete defender;
        delete attacker;
        delete weapon;
    }
};

TEST_F(WeaponFixture, Contains) {
    EXPECT_EQ(weapon->contains("mocny strzal"), true);
    EXPECT_EQ(weapon->contains("random"), false);
}

TEST_F(WeaponFixture, Process) {
    EXPECT_EQ(defender->health, 50);
    weapon->process_move("mocny strzal", *attacker, *defender, 10);
    EXPECT_EQ(defender->health, 29);

}

