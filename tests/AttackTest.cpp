#include "lib/googletest/include/gtest/gtest.h"
#include "lib/googlemock/include/gmock/gmock.h"

#include "../battlefield/Battlefield.h"



class AttackFixture : public ::testing::Test {
protected:
    Attack<int>* attack;
    Battlefield<int> battlefield;
    WoArcher<int>* attacker;
    WomKnight<int>* defender;

    void SetUp() override {
        attack = new Attack<int>(physical, 0, 5, 2, 2, 2, 2);
        attacker = new WoArcher<int>("TestWombat", new Bow<int>(), new ThiefArmor<int>(), battlefield);
        defender = new WomKnight<int>("TestWombat", new Falchion<int>(), new HavelArmor<int>(), battlefield);

    }

    void TearDown() override {
        delete attack;
        delete defender;
        delete attacker;
    }
};

TEST_F(AttackFixture, CheckAvailable) {
    EXPECT_EQ(attack->check_available(2), true);
    EXPECT_EQ(attack->check_available(0), true);
    EXPECT_EQ(attack->check_available(5), true);
    EXPECT_EQ(attack->check_available(1), true);
    EXPECT_EQ(attack->check_available(4), true);
    EXPECT_EQ(attack->check_available(-1), false);
    EXPECT_EQ(attack->check_available(6), false);
    EXPECT_EQ(attack->check_available(10), false);

}

TEST_F(AttackFixture, CheckResources) {
    EXPECT_EQ(attack->check_resources(*attacker), true);
    attacker->current_mana = 0;
    EXPECT_EQ(attack->check_resources(*attacker), false);

}

TEST_F(AttackFixture, DealDmg) {
    EXPECT_EQ(defender->health, 50);
    attack->deal_dmg(*attacker, *defender);
    EXPECT_EQ(defender->health, 41);
}

TEST_F(AttackFixture, GetInfo) {
    EXPECT_EQ(attack->info(), "zasieg: 0-5, koszt many: 2 ,koszt staminy: 2");

}

