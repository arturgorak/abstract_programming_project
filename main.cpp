#include <iostream>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <string_view>
#include <ranges>
#include <bits/stdc++.h>


//enum dmg_type{
//    physical = 0,
//    magical = 1
//};
//
//
//
//enum status{
//    stunned = 0,
//    set_on_fire = 1,
//    bleeding = 2
//};

//class Weapon{
//    int range;
//    dmg_type dmg_type;
//    int stamina_cost;
//    int mana_cost;
//};


class Battlefield;

class Armour{
    int magical_def;
public:
    int physical_def;
};

class Wombat;

class Weapon{
public:
//    virtual void low_range_attack (Wombat& attacker, Wombat& defender){}
//    virtual void mid_range_attack (Wombat& attacker, Wombat& defender){}
//    virtual void long_range_attack (Wombat& attacker, Wombat& defender){}
};

class Wombat{
public:
    std::string name;
    int health;
    int current_stamina;
    int stamina;
    int current_mana;
    int mana;
    int strength;
    int dex;
    int luck;

    Armour armour{};
//    status* statuses{};
    Battlefield& battlefield;

    int stamina_restore = 5;
    int mana_restore = 5;

    Wombat(std::string n, int h, int s, int m, int str, int d, int l, Battlefield &b) :name(std::move(n)), health(h), current_stamina(1), stamina(s), current_mana(1), mana(m), strength(str), dex(d), luck(l), battlefield(b) {}

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

    bool operator<(const Wombat& other) const {
        if (this->dex != other.dex) {
            return this->dex > other.dex;
        }
        if (this->strength != other.strength) {
            return this->strength > other.strength;
        }
        return this->stamina > other.stamina;
    }

    void wait(){
        stats();
        std::cout << "Wywoluje wait" << std::endl;
        current_stamina = std::min(stamina, current_stamina + stamina_restore);
        current_mana = std::min(mana, current_mana + mana_restore);
    }

    void reduce_distance();
    void increase_distance();

    void base_moves_explain(){
        std::cout
        << "help - wytłumaczenie ruchow" << std::endl
        << "wait - regeneracja " << stamina_restore << " punktow staminy oraz " << mana_restore << " punktow many" << std::endl
        << "reduce distance - zmniejszenie odległości do przeciwnika o " << dex << " jednostek" << std::endl
        << "increase distance - zwiekszenie odległości do przeciwnika o " << dex << " jednostek" << std::endl;
    }

    virtual void print_moves(){
        std:: cout << "Wybierz jeden z ponizszych ruchow" << std:: endl;
        std::cout
                << "help" << std::endl
                << "wait" << std::endl
                << "reduce distance" << std::endl
                << "increase distance" << std::endl;
    }
    virtual void choose_move(Wombat& defender, int distance){};
//    virtual void print_moves() = 0;

    bool is_dead(){
        return(health <= 0);
    }
};

class Attack{
public:
    std::string name;
    int min_range;
    int max_range;
    int scaling_dex;
    int scaling_strength;
    int mana_cost;
    int stamina_cost;


    Attack(
            std::string name,
            int min_range,
            int max_range,
            int scaling_dex,
            int scaling_strength,
            int mana_cost,
            int stamina_cost) : name(std::move(name)), min_range(min_range), max_range(max_range), scaling_dex(scaling_dex), scaling_strength(scaling_strength), mana_cost(mana_cost), stamina_cost(stamina_cost) {}

    bool check_available(int distance){
        return distance >= min_range and distance <= max_range;
    }

    bool check_resources(Wombat& attacker){
        return mana_cost <= attacker.mana and stamina_cost <= attacker.stamina;
    }

    void deal_dmg(Wombat& attacker, Wombat& defender){
        int dmg = attacker.strength * scaling_strength + attacker.dex * scaling_dex;
        attacker.stamina -= stamina_cost;
        attacker.mana -= mana_cost;
        defender.health -= dmg;
    }
    void print(){
        std::cout
        << "Name " << name << std::endl
        << "Min range " << min_range << std::endl
        << "Max range " << max_range << std::endl
        << "Stamina " << stamina_cost << std::endl;
    }

    void process_attack(Wombat& attacker, Wombat& defender, int distance){
        print();
        if(not check_available(distance)){
            std::cout << "Chybiono: Cel znajduje sie w odleglosci " << distance << ", podczas gdy atak ma zasieg " << min_range << " do " << max_range << std:: endl;
            return;

        }

        if(not check_resources(attacker)){
            std::cout << "Nie posiadasz zasobów aby uzyc tego ataku. Wymaga on " << stamina_cost << " punktow staminy i " << mana_cost << " punktow many"<< std:: endl;
            std::cout << "Niestety tracisz ruch" << std:: endl;
            return;

        }

        deal_dmg(attacker, defender);

    }

    std::string get_name(){
        std::cout << "yo1" << std::endl;
        return name;
    }

};


class Sword : public Weapon {
public:
    std::map<std::string, Attack> attacks{
            {"Blisko", Attack("Blisko", 0, 4, 4, 0, 0, 3)},
            {"Srednio", Attack("Srednio", 3, 6, 2, 2, 0, 5)},
            {"Daleko", Attack("Daleko", 5, 9, 0, 4, 0, 7)},
    };


    void print_attacks_names() {
        std::cout << "Attack names:" << std::endl;
        for (const auto& elem : this->attacks) {
            std::cout << elem.first << std::endl;
        }
    }

    [[nodiscard]] bool contains(const std::string& key) const {
        return (attacks.contains(key));
    }

    void process_move(const std::string& key, Wombat& attacker, Wombat& defender, int distance ){
        this->attacks.at(key).process_attack(attacker, defender, distance);
    }

};


class Catalyst: public Weapon{
public:

    void long_range_attack(Wombat& attacker, Wombat& defender){
        int dmg = attacker.strength * 4;
        attacker.stamina -= 6;
        defender.health -= dmg;

    }
    void mid_range_attack(Wombat& attacker, Wombat& defender) {
        int dmg = attacker.strength * 2 + attacker.dex * 2;
        attacker.stamina -= 4;
        defender.health -= dmg;

    }
    void low_range_attack(Wombat& attacker, Wombat& defender) {

        int dmg = attacker.dex * 4;
        attacker.stamina -= 2;
        defender.health -= dmg;

    }
};

class WomKnight : public Wombat{
//    std::map<std::string, int> moves{{"Help", 0}, {"Mage", 1}};
    Sword* weapon;

public:
    WomKnight(std::string n, Sword* w, Battlefield& b) : Wombat(n, 10, 10, 5,5, 4, 2, b), weapon(w) {}

    void print_moves() override{
//        print_base_moves();
//        std::cout
//                << "low-range attack" << std::endl
//                << "mid-range attack" << std::endl
//                << "long-range attack" << std::endl;

//        for (const auto& entry : moves) {
//            std::cout << entry.first << std::endl;
//
//
//        }
    }


    void choose_move(Wombat& defender, int distance){
        std::map<std::string, std::function<void()>> moves{
                {"wait", [this]() { wait(); }},
                {"reduce-distance", [this]() { reduce_distance(); }},
                {"increase-distance", [this]() { increase_distance(); }},

        };



        std::cout << "Available moves:" << std::endl;
        for (const auto& move : moves) {
            std::cout << move.first << std::endl;
        }

        this->weapon->print_attacks_names();

        while (true) {
            std::string chosen_move;
            std::cout << "Wybierz ruch: ";
            std::cin >> chosen_move;

            std::cout << std::endl << "Wybrano ruch: " << chosen_move << std::endl;

            if (moves.contains(chosen_move)) {
                moves[chosen_move]();
                break;

            } else if(weapon->contains(chosen_move)){
                this->weapon->process_move(chosen_move, *this, defender, distance);
                break;

            } else {
                std::cout << "Prosze podac ponownie, ruch " << chosen_move << " jest niepoprawny" << std::endl;
            }
        }



    }
};

class WoMage : public Wombat{
public:
    WoMage(std::string n, Catalyst& weapon1, Battlefield& b) : Wombat(n, 7, 7, 10,2, 6, 3, b) {}

    void print_moves() override{
//        print_base_moves();
        std::cout

                << "Mage moves" << std::endl;

//        for (const auto& entry : moves) {
//            std::cout << entry.first << std::endl;
//        }
    }
};


enum wombat_classes{
    knight = 0,
    mage = 1
};

class Battlefield{

    int current_distance = 10;
    int max_distance = 30;

    std::map<std::string, int> classes{{"Knight", 0}, {"Mage", 1}};



public:
    void reduce_distance(int distance){
        current_distance = std::max(0, current_distance - distance);
    }

    void increase_distance(int distance){
        current_distance = std::min(max_distance, current_distance + distance);
    }
    Wombat* create_wombat(const std::string& name){
        auto kv = std::views::keys(classes);

        while(true) {
            std::cout << "Prosze wybrac jedna z istniejacych klas: " << std::endl;
            for (auto x: kv) {
                std::cout << x << std::endl;
            }
            std::string chosen_class;
            std::cin >> chosen_class;
            std::cout << std::endl;

            if (classes.contains(chosen_class)) {

                switch (classes[chosen_class]) {
                    case 0: {
                        std::cout << "You choose Knight" << std::endl;
                        auto sword = new Sword();
                        return new WomKnight(name, sword, *this);
                    }
                    case 1: {
                        std::cout << "You choose Mage" << std::endl;
                        Catalyst catalyst;
                        return new WoMage(name, catalyst, *this);
                    }

                }

            } else {
                std::cout << "Prosze podac ponownie, klasa " << chosen_class << " jest niepoprawna"<< std::endl;
            }

        }

    }

    void game(){
        std::cout << "-------PLAYER 1------" << std::endl;
        Wombat* player1 = create_wombat("Player 1");


        std::cout << "-------PLAYER 2------" << std::endl;

        Wombat* player2 = create_wombat("Player 2");

        std::cout << "-------FIGHT------" << std::endl;

        while(true){
            std::cout << "Tura " << player1->name << std::endl;
            player1->choose_move(*player2, current_distance);

            player2->stats();
            if(player2->is_dead()){
                std::cout << "Wygral gracz: " << player1->name << std::endl;
                break;
            }

            std::cout << "Tura " << player2->name << std::endl;
            player2->choose_move(*player1, current_distance);


            if(player1->is_dead()){
                std::cout << "Wygral gracz: " << player2->name << std::endl;
                break;
            }
        }


    }
};

inline void Wombat::reduce_distance() {
    std::cout << "Wywoluje reduce" << std::endl;

    battlefield.reduce_distance(dex);
}
inline void Wombat::increase_distance() {
    std::cout << "Wywoluje increase" << std::endl;

    battlefield.increase_distance(dex);
}

int main() {

    Battlefield battlefield;
    battlefield.game();
}
