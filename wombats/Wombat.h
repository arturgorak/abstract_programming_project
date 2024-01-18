#pragma once

#include <iostream>
#include <map>
#include <string>
#include <functional>

#include "../equipment/Armor.h"

template <SubAddMult T>
class Wombat{
public:
    std::string name;
    T health;
    T current_stamina;
    T stamina;
    T current_mana;
    T mana;
    T strength;
    T dex;
    T luck;

    std::map<std::string, std::function<void()>> moves{
            {"czekaj", [this]() { wait(); }},
            {"zmniejsz dystans", [this]() { this->reduce_distance(); }},
            {"zwieksz dystans", [this]() { this->increase_distance(); }},

    };

    std::map<std::string, std::function<void()>> info_moves{
            {"statystyki", [this]() { stats(); }},
            {"ataki", [this]() { this->weapon->print_attacks_stats(); }},

    };

    Battlefield<T>& battlefield;
    Armor<T>* armor;
    Weapon<T>* weapon;

    T stamina_restore = 5;
    T mana_restore = 5;

    Wombat(std::string n, T h, T s, T m, T str, T d, T l, Weapon<T>* w, Armor<T>* a, Battlefield<T> &b) : name(std::move(n)), health(h), current_stamina(5), stamina(s), current_mana(5), mana(m), strength(str), dex(d), luck(l), weapon(w), armor(a), battlefield(b) {}

    void stats() const{
        std::cout
                << "Name: " << name << std::endl
                << "Health: " << health << std::endl
                << "Stamina: " << current_stamina << "/" <<  stamina << std::endl
                << "Mana: " << current_mana << "/" << mana << std::endl
                << "Str: " << strength << std::endl
                << "Dex: " << dex << std::endl
                << "Luck: " << luck << std::endl
                << std::endl;
    }

    bool operator>(const Wombat& other) const {
        if (this->dex != other.dex) {
            return this->dex > other.dex;
        }
        if (this->strength != other.strength) {
            return this->strength > other.strength;
        }
        return this->stamina > other.stamina;
    }

    void wait(){
        current_stamina = std::min(stamina, current_stamina + stamina_restore);
        current_mana = std::min(mana, current_mana + mana_restore);
    }


    virtual void reduce_distance() {
        battlefield.reduce_distance(dex);
    }

    virtual void increase_distance() {
        battlefield.increase_distance(dex);
    }

    virtual void print_moves(){
        std:: cout << "------------------------------------------------" << std:: endl;

        std:: cout << "Wybierz jeden z ponizszych ruchow:" << std:: endl;
        std:: cout << "Ruchy nie konczace tury:" << std:: endl;

        for (const auto& move : info_moves) {
            std::cout << move.first << std::endl;
        }
        std:: cout  << std:: endl;
        std:: cout << "Ponizsze ruchy koncza ture:" << std:: endl;

        for (const auto& move : moves) {
            std::cout << move.first << std::endl;
        }

    }

    bool is_dead(){
        return(health <= 0);
    }

    virtual void passive(){}

    void choose_move(Wombat<T>& defender, int distance){

        this->print_moves();
        this->weapon->print_attacks_names();

        while (true) {
            std::string chosen_move;
            std::cout << "Wybierz ruch: ";
            std::getline(std::cin, chosen_move);

            std::cout << std::endl << "Wybrano ruch: " << chosen_move << std::endl;
            if (this->info_moves.contains(chosen_move)){
                this->info_moves[chosen_move]();
            } else if (this->moves.contains(chosen_move)) {
                this->moves[chosen_move]();
                this->passive();
                break;

            } else if(weapon->contains(chosen_move)){
                this->weapon->process_move(chosen_move, *this, defender, distance);
                this->passive();
                break;

            } else {
                std::cout << "Prosze podac ponownie, ruch " << chosen_move << " jest niepoprawny" << std::endl;
            }
        }

    }



};

