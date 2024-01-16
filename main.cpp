#include <iostream>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <string_view>
#include <ranges>
#include <bits/stdc++.h>

template <typename T>
concept Numeric = std::is_arithmetic<T>::value;

template <Numeric T>
class Battlefield;

class Armour{
    int magical_def;
public:
    int physical_def;
};


template <Numeric T>
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
            {"wait", [this]() { wait(); }},
            {"reduce-distance", [this]() { reduce_distance(); }},
            {"increase-distance", [this]() { increase_distance(); }},

    };

    Armour armour{};
//    status* statuses{};
    Battlefield<T>& battlefield;

    T stamina_restore = 5;
    T mana_restore = 5;

    Wombat(std::string n, T h, T s, T m, T str, T d, T l, Battlefield<T> &b) :name(std::move(n)), health(h), current_stamina(1), stamina(s), current_mana(1), mana(m), strength(str), dex(d), luck(l), battlefield(b) {}

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
        for (const auto& move : moves) {
            std::cout << move.first << std::endl;
        }

    }
    virtual void choose_move(Wombat& defender, int distance){};

    bool is_dead(){
        return(health <= 0);
    }
};

template <Numeric T>
class Attack{
public:
    int min_range;
    int max_range;
    T scaling_dex;
    T scaling_strength;
    T mana_cost;
    T stamina_cost;


    Attack(
            int min_range,
            int max_range,
            int scaling_dex,
            int scaling_strength,
            int mana_cost,
            int stamina_cost) : min_range(min_range), max_range(max_range), scaling_dex(scaling_dex), scaling_strength(scaling_strength), mana_cost(mana_cost), stamina_cost(stamina_cost) {}

    bool check_available(int distance){
        return distance >= min_range and distance <= max_range;
    }

    bool check_resources(Wombat<T>& attacker){
        return mana_cost <= attacker.mana and stamina_cost <= attacker.stamina;
    }

    void deal_dmg(Wombat<T>& attacker, Wombat<T>& defender){
        T dmg = attacker.strength * scaling_strength + attacker.dex * scaling_dex;
        defender.health -= dmg;
    }

    void process_attack(Wombat<T>& attacker, Wombat<T>& defender, int distance){
        if(not check_resources(attacker)){
            std::cout << "Nie posiadasz zasobow aby uzyc tego ataku. Wymaga on " << stamina_cost << " punktow staminy i " << mana_cost << " punktow many"<< std:: endl;
            std::cout << "Niestety tracisz ruch" << std:: endl;
            return;

        }

        attacker.stamina -= stamina_cost;
        attacker.mana -= mana_cost;

        if(not check_available(distance)){
            std::cout << "Chybiono: Cel znajduje sie w odleglosci " << distance << ", podczas gdy atak ma zasieg " << min_range << " do " << max_range << std:: endl;
            return;

        }

        deal_dmg(attacker, defender);

    }

};

template <Numeric T>
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

    [[nodiscard]] bool contains(const std::string& key) const {
        return (attacks.contains(key));
    }

    void process_move(const std::string& key, Wombat<T>& attacker, Wombat<T>& defender, int distance ){
        this->attacks.at(key).process_attack(attacker, defender, distance);
    }

};

template <Numeric T>
class Sword : public Weapon<T> {
public:
    std::map<std::string, Attack<T>> attacks{
            {"Blisko", Attack<T>(0, 4, 4, 0, 0, 3)},
            {"Srednio", Attack<T>(3, 6, 2, 2, 0, 5)},
            {"Daleko", Attack<T>(5, 9, 0, 4, 0, 7)},
    };

    Sword() : Weapon<T>(attacks) {}

};

template <Numeric T>
class Bow: public Weapon<T>{
public:

    std::map<std::string, Attack<T>> attacks{
            {"Slaby", Attack<T>(7, 13, 6, 0, 0, 3)},
            {"Mocny", Attack<T>( 10, 17, 4, 2, 0, 5)},
            {"Sniper", Attack<T>(15, 20, 4, 4, 0, 7)},
    };

    Bow() : Weapon<T>(attacks) {}

};

template <Numeric T>
class WomKnight : public Wombat<T>{
    Sword<T>* weapon;

public:
    WomKnight(std::string n, Sword<T>* w, Battlefield<T>& b) : Wombat<T>(n, 10, 10, 5,5, 4, 2, b), weapon(w) {}

    void reduce_distance();

    void choose_move(Wombat<T>& defender, int distance){
        std::cout << "status" << std::endl;
        this->print_moves();
        this->weapon->print_attacks_names();

        while (true) {
            std::string chosen_move;
            std::cout << "Wybierz ruch: ";
            std::cin >> chosen_move;

            std::cout << std::endl << "Wybrano ruch: " << chosen_move << std::endl;

            if(chosen_move == "status"){ //TODO repair
                this->stats();
                std::cout << "Prosze podac ponownie ruch"  << std::endl;
            } else if (this->moves.contains(chosen_move)) {
                this->moves[chosen_move]();
                this->reduce_distance();
                break;

            } else if(weapon->contains(chosen_move)){
                this->weapon->process_move(chosen_move, *this, defender, distance);
                this->reduce_distance();
                break;

            } else {
                std::cout << "Prosze podac ponownie, ruch " << chosen_move << " jest niepoprawny" << std::endl;
            }
        }

    }


};

template <Numeric T>
class WoArcher : public Wombat<T>{
public:
    Bow<T>* weapon;
    WoArcher(std::string n, Bow<T>* w, Battlefield<T>& b) : Wombat<T>(n, 7, 7, 10,2, 6, 3, b), weapon(w) {}

    void increase_distance();

    void choose_move(Wombat<T>& defender, int distance){
        this->print_moves();
        this->weapon->print_attacks_names();

        while (true) {
            std::string chosen_move;
            std::cout << "Wybierz ruch: ";
            std::cin >> chosen_move;

            std::cout << std::endl << "Wybrano ruch: " << chosen_move << std::endl;

            if (this->moves.contains(chosen_move)) {
                this->moves[chosen_move]();
                this->increase_distance();
                break;

            } else if(weapon->contains(chosen_move)){
                this->weapon->process_move(chosen_move, *this, defender, distance);
                this->increase_distance();
                break;

            } else {
                std::cout << "Prosze podac ponownie, ruch " << chosen_move << " jest niepoprawny" << std::endl;
            }
        }

    }


};

template <Numeric T>
class Battlefield{

    int current_distance = 10;
    int max_distance = 30;

    std::map<std::string, int> classes{{"Knight", 0}, {"Archer", 1}};



public:
    void reduce_distance(int distance){
        current_distance = std::max(0, current_distance - distance);
    }

    void increase_distance(int distance){
        current_distance = std::min(max_distance, current_distance + distance);
    }
    Wombat<T>* create_wombat(const std::string& name){
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
                        auto sword = new Sword<T>();
                        return new WomKnight(name, sword, *this);
                    }
                    case 1: {
                        std::cout << "You choose Archer" << std::endl;
                        auto bow = new Bow<T>();
                        return new WoArcher(name, bow, *this);
                    }

                }

            } else {
                std::cout << "Prosze podac ponownie, klasa " << chosen_class << " jest niepoprawna"<< std::endl;
            }

        }

    }

    void game(){
        std::cout << "-------PLAYER 1------" << std::endl;
        Wombat<T>* player1 = create_wombat("Player 1");


        std::cout << "-------PLAYER 2------" << std::endl;

        Wombat<T>* player2 = create_wombat("Player 2");

        std::cout << "-------FIGHT------" << std::endl;

        while(true){
            std::cout << "Tura " << player1->name << std::endl;
            std::cout << "Obecny dystacs wynosi: " << current_distance << std::endl;

            player1->choose_move(*player2, current_distance);

            if(player2->is_dead()){
                std::cout << "Wygral gracz: " << player1->name << std::endl;
                break;
            }

            std::cout << "Tura " << player2->name << std::endl;
            std::cout << "Obecny dystacs wynosi: " << current_distance << std::endl;
            player2->choose_move(*player1, current_distance);


            if(player1->is_dead()){
                std::cout << "Wygral gracz: " << player2->name << std::endl;
                break;
            }
        }


    }
};

template <Numeric T>
inline void Wombat<T>::reduce_distance() {
    battlefield.reduce_distance(dex);
}

template <Numeric T>
inline void Wombat<T>::increase_distance() {
    battlefield.increase_distance(dex);
}


template <Numeric T>
inline void WomKnight<T>::reduce_distance() {
    this->battlefield.reduce_distance(this->dex * 2);
}

template <Numeric T>
inline void WoArcher<T>::increase_distance() {
    this->battlefield.increase_distance(this->dex * 2);
}

int main() {
    Battlefield<int> battlefield;
    battlefield.game();
}
