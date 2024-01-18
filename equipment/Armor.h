//
// Created by Artem on 17.01.2024.
//

#ifndef WOMBAT_ARMOR_H
#define WOMBAT_ARMOR_H

#include <algorithm>
#include "../utils/Concepts.h"

template <SubAddMult T>
class Battlefield;

template <SubAddMult T>
class Wombat;

template <SubAddMult T>
class Weapon;

template <SubAdd T>
class Armor{
public:
    T magical_def;
    T physical_def;

    Armor(T md, T pd): magical_def(md), physical_def(pd) {}

    virtual void effect(Wombat<T>& defender){};
};

template <SubAdd T>
class LightArmor : public Armor<T>{
public:
    LightArmor(T md, T pd): Armor<T>(md, pd){}
};

template <SubAdd T>
class ThiefArmor : public LightArmor<T>{
public:
    ThiefArmor(): LightArmor<T>(2, 4){}
    void effect(Wombat<T> &wombat) {
        wombat.dex += 15;
        wombat.health = std::max(wombat.health - 10, 1);
        wombat.current_stamina = std::min(wombat.stamina, wombat.stamina + 5);
    }
};


template <SubAdd T>
class HunterArmor : public LightArmor<T>{
public:
    HunterArmor(): LightArmor<T>(3, 3){}
    void effect(Wombat<T> &wombat) {
        wombat.dex += 5;
        wombat.current_stamina = std::min(wombat.stamina, wombat.stamina + 5);
    }
};

template <SubAdd T>
class HeavyArmor: public Armor<T>{
public:
    HeavyArmor(T md, T pd): Armor<T>(md, pd){}

};


template <SubAdd T>
class HavelArmor: public HeavyArmor<T>{
public:
    HavelArmor(): HeavyArmor<T>(7, 7){}
    void effect(Wombat<T> &wombat) {
        wombat.dex = std::max(0, wombat.dex - 5);
        wombat.strength += 4;
        wombat.current_stamina = std::max(0, wombat.stamina - 5);
    }

};

template <SubAdd T>
class EliteKnightArmor: public HeavyArmor<T>{
public:
    EliteKnightArmor(): HeavyArmor<T>(5, 5){}
    void effect(Wombat<T> &wombat) {
        wombat.dex = std::max(0, wombat.dex - 4);
        wombat.health += 15;
        wombat.current_stamina = std::max(0, wombat.stamina - 5);
    }

};


#endif //WOMBAT_ARMOR_H
