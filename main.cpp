#include <iostream>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <string_view>
#include <ranges>


enum dmg_type{
    physical = 0,
    magical = 1
};



enum status{
    stunned = 0,
    set_on_fire = 1,
    bleeding = 2
};

//class Weapon{
//    int range;
//    dmg_type dmg_type;
//    int stamina_cost;
//    int mana_cost;
//};




class Armour{
    int magical_def;
public:
    int physical_def;
};

class Wombat;

class Weapon{
public:
    virtual void low_range_attack (Wombat& attacker, Wombat& defender){}
    virtual void mid_range_attack (Wombat& attacker, Wombat& defender){}
    virtual void long_range_attack (Wombat& attacker, Wombat& defender){}
};

class Wombat{
public:
    std::string name;
    float health;
    int stamina;
    int mana;
    int strength;
    int dex;
    int luck;
    Armour armour{};
    status* statuses{};
    Weapon& weapon;

    Wombat(std::string n, int h, int s, int m, int str, int d, int l, Weapon& w) :name(std::move(n)), health(h), stamina(s), mana(m), strength(str), dex(d), luck(l), weapon(w) {}

    void stats() const{
        std::cout
        << "Name: " << name << std::endl
        << "Health: " << health << std::endl
        << "Stamina: " << stamina << std::endl
        << "Mana: " << mana << std::endl
        << "Str: " << strength << std::endl
        << "Dex: " << dex << std::endl
        << "Luck: " << luck << std::endl
        << std::endl;
    }

    bool is_dead(){
        return(health <= 0);
    }
};


class Sword: public Weapon{
public:

    void long_range_attack(Wombat& attacker, Wombat& defender) override{
        float dmg = attacker.strength * 4;
        attacker.stamina -= 6;
        defender.health -= dmg;

    }
    void mid_range_attack(Wombat& attacker, Wombat& defender) override{
        float dmg = attacker.strength * 2 + attacker.dex * 2;
        attacker.stamina -= 4;
        defender.health -= dmg;

    }
    void low_range_attack(Wombat& attacker, Wombat& defender) override{
        float dmg = attacker.dex * 4;
        attacker.stamina -= 2;
        defender.health -= dmg;

    }
};

class WomKnight : public Wombat{
public:
    WomKnight(Weapon& weapon) : Wombat("Knight", 10, 10, 5,5, 4, 2, weapon) {
    }
};

class WoMage : public Wombat{
public:
    WoMage(Weapon& weapon1) : Wombat("Mage", 7, 7, 10,2, 6, 3, weapon1) {}
};


enum wombat_classes{
    knight = 0,
    mage = 1
};

class Battlefield{

    int current_distance = 5;
    int max_distance = 30;
    std::map<std::string, int> classes{{"Knight", 0}, {"Mage", 1}};



public:
    Wombat create_wombat(){
        auto kv = std::views::keys(classes);
        std::vector<std::string> keys{ kv.begin(), kv.end() };

        while(true) {
            std::cout << "Prosze wybrac jedna z istniejacych klas: " << std::endl;
            for (auto x: kv) {
                std::cout << x << std::endl;
            }
            std::string chosen_class;
            std::cin >> chosen_class;
            std::cout << std::endl;

            if (classes.contains(chosen_class)) {
                std::cout << "Znaleziono";
                Sword sword;
                return WomKnight(sword);
            } else {
                std::cout << "Prosze podac ponownie, klasa " << chosen_class << " jest niepoprawna"<< std::endl;
            }

        }

    }

    void game(){
        create_wombat();
    }
};



int main() {

    Battlefield battlefield;
    battlefield.game();
}
