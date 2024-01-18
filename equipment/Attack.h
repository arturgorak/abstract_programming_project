//
// Created by Artem on 17.01.2024.
//

#ifndef WOMBAT_ATTACK_H
#define WOMBAT_ATTACK_H
#include "../wombats/Wombat.h"


enum DamageType {magical, physical};

template <SubAddMult T>
class Attack{
public:
    int min_range;
    int max_range;
    DamageType type;
    T scaling_dex;
    T scaling_strength;
    T mana_cost;
    T stamina_cost;


    Attack(
            DamageType type,
            int min_range,
            int max_range,
            T scaling_dex,
            T scaling_strength,
            T mana_cost,
            T stamina_cost) :type(type), min_range(min_range), max_range(max_range), scaling_dex(scaling_dex), scaling_strength(scaling_strength), mana_cost(mana_cost), stamina_cost(stamina_cost) {}

    bool check_available(int distance){
        return distance >= min_range and distance <= max_range;
    }

    bool check_resources(Wombat<T>& attacker){
        return mana_cost <= attacker.current_mana and stamina_cost <= attacker.current_stamina;
    }

    virtual void deal_dmg(Wombat<T>& attacker, Wombat<T>& defender){
        T def = 0;
        if(type==physical){
            def = defender.armor->physical_def;
        } else {
            def = defender.armor->magical_def;
        }
        T dmg = std::max(attacker.strength * scaling_strength + attacker.dex * scaling_dex - def, 0);
        std::cout << "Zadales " << dmg << " obrazen" << std::endl;
        defender.health -= dmg;
    }

    void process_attack(Wombat<T>& attacker, Wombat<T>& defender, int distance){
        if(not check_resources(attacker)){
            std::cout << "Nie posiadasz zasobow aby uzyc tego ataku. Wymaga on " << stamina_cost << " punktow staminy i " << mana_cost << " punktow many"<< std:: endl;
            std::cout << "Niestety tracisz ruch" << std:: endl;
            return;

        }

        attacker.current_stamina -= stamina_cost;
        attacker.current_mana -= mana_cost;

        if(not check_available(distance)){
            std::cout << "Chybiono: Cel znajduje sie w odleglosci " << distance << ", podczas gdy atak ma zasieg " << min_range << " do " << max_range << std:: endl;
            return;

        }

        this->deal_dmg(attacker, defender);

    }

    [[nodiscard]] std::string info() const {
        return "zasieg: " + std::to_string(min_range) + "-" + std::to_string(max_range) + ", koszt many: " + std::to_string(mana_cost) + " ,koszt staminy: " + std::to_string(stamina_cost);
    }

};

#endif //WOMBAT_ATTACK_H
