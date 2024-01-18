//
// Created by Artem on 17.01.2024.
//

#ifndef WOMBAT_WEAPON_H
#define WOMBAT_WEAPON_H
#include "Attack.h"

template <SubAddMult T>
class Weapon{
    std::map<std::string, Attack<T>>& attacks;
public:
    explicit Weapon(std::map<std::string, Attack<T>> &attacks) : attacks(attacks) {}

    void print_attacks_names() {
        std::cout << std::endl << "Dostepne ataki:" << std::endl;
        for (const auto& elem : this->attacks) {
            std::cout << elem.first << std::endl;
        }
    }

    void print_attacks_stats() {
        for (const auto& elem : this->attacks) {
            std::cout << elem.first << " => " << elem.second.info() << std::endl;
        }
    }

    [[nodiscard]] bool contains(const std::string& key) const {
        return (attacks.contains(key));
    }

    void process_move(const std::string& key, Wombat<T>& attacker, Wombat<T>& defender, int distance ){
        this->attacks.at(key).process_attack(attacker, defender, distance);
    }

};

template <SubAddMult T>
class ColdWeapon : public Weapon<T> {
public:
    explicit ColdWeapon(std::map<std::string, Attack<T>>& attacks) : Weapon<T>(attacks) {}
};

template <SubAddMult T>
class Falchion: public ColdWeapon<T>{
public:
    std::map<std::string, Attack<T>> attacks{
            {"bliski atak", Attack<T>(physical, 0, 4, 4, 0, 0, 3)},
            {"magiczny atak", Attack<T>(magical,3, 6, 2, 2, 1, 5)},
            {"daleki atak", Attack<T>(physical,5, 9, 0, 4, 0, 7)},
    };

    Falchion(): ColdWeapon<T>(attacks){}

};


template <SubAddMult T>
class Morgenstern: public ColdWeapon<T>{
public:
    std::map<std::string, Attack<T>> attacks{
            {"bliski zamach", Attack<T>(physical, 0, 5, 4, 3, 0, 3)},
            {"dalekie uderzenie", Attack<T>(physical,4, 7, 2, 5, 0, 7)},
    };

    Morgenstern(): ColdWeapon<T>(attacks){}

};

template <SubAddMult T>
class Koncerz: public ColdWeapon<T>{
public:
    std::map<std::string, Attack<T>> attacks{
            {"szybkie ciecie", Attack<T>(magical, 0, 8, 2, 2, 0, 2)},
            {"daleki atak", Attack<T>(magical,8, 13, 1, 6, 2, 6)},
    };

    Koncerz(): ColdWeapon<T>(attacks){}

};

template <SubAddMult T>
class DistanceWeapon: public Weapon<T>{
public:
    explicit DistanceWeapon(std::map<std::string, Attack<T>>& attacks) : Weapon<T>(attacks) {}
};

template <SubAddMult T>
class Bow: public DistanceWeapon<T>{
public:
    std::map<std::string, Attack<T>> attacks{
        {"slaby strzal", Attack<T>(physical, 7, 13, 6, 0, 0, 3)},
        {"mocny strzal", Attack<T>(physical, 10, 17, 4, 2, 0, 5)},
        {"snajperski strzal", Attack<T>(physical,15, 20, 4, 4, 0, 7)},
    };
    Bow() : DistanceWeapon<T>(attacks) {}
};

template <SubAddMult T>
class Slingshot: public DistanceWeapon<T>{
public:
    std::map<std::string, Attack<T>> attacks{
        {"bliski strzal", Attack<T>(physical, 0, 10, 6, 0, 0, 3)},
        {"naladowany strzal", Attack<T>(physical, 7, 17, 6, 2, 0, 5)},
        {"naladowany magiczny strzal", Attack<T>(magical,7, 20, 6, 4, 2, 5)},
    };
    Slingshot() : DistanceWeapon<T>(attacks) {}
};



#endif //WOMBAT_WEAPON_H
