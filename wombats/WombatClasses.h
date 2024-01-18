//
// Created by Artem on 17.01.2024.
//

#ifndef WOMBAT_WOMBATCLASSES_H
#define WOMBAT_WOMBATCLASSES_H

#include "../equipment/Weapon.h"


template <SubAddMult T>
class WomKnight : public Wombat<T>{
public:
    WomKnight(std::string n, ColdWeapon<T>* w, HeavyArmor<T>* a, Battlefield<T>& b) : Wombat<T>(n, 50, 10, 5, 5, 4, 2, w, a, b) {}

    void passive() override{
        this->reduce_distance();
    }


    void reduce_distance() override {
        this->battlefield.reduce_distance(this->dex * 2);
    }

};

template <SubAddMult T>
class WoArcher : public Wombat<T>{
public:
    WoArcher(std::string n, DistanceWeapon<T>* w, LightArmor<T>* a, Battlefield<T>& b) : Wombat<T>(n, 45, 7, 10, 2, 6, 3, w, a, b) {}

    void increase_distance() override {
        this->battlefield.increase_distance(this->dex * 2);
    }

};


#endif //WOMBAT_WOMBATCLASSES_H
