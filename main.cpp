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

template <typename T>
concept Add= requires ( T a , T b ) {
    a + b;
};

template <typename T>
concept Sub = requires ( T a , T b ) {
    a - b;
};

template <typename T>
concept SubAdd = requires (T a) { Add<T> && Sub<T>; };

template <typename T>
concept Mult = requires ( T a , T b ) {
    a*b;
};

template <typename T>
concept SubAddMult = requires (T a) { Add<T> && Sub<T> && Mult<T>; };


enum DamageType {magical, physical};

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
    void effect(Wombat<T>& wombat);
};


template <SubAdd T>
class HunterArmor : public LightArmor<T>{
public:
    HunterArmor(): LightArmor<T>(3, 3){}
    void effect(Wombat<T>& wombat);
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
    void effect(Wombat<T>& wombat);

};

template <SubAdd T>
class EliteKnightArmor: public HeavyArmor<T>{
public:
    EliteKnightArmor(): HeavyArmor<T>(5, 5){}
    void effect(Wombat<T>& wombat);

};


template <typename T, typename U>
concept WeaponArmor = (std::is_same_v<T, Armor<U>*> || std::is_same_v<T, Weapon<U>*>) && SubAddMult<U>;



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

    Wombat(std::string n, T h, T s, T m, T str, T d, T l, Weapon<T>* w, Armor<T>* a, Battlefield<T> &b) : name(std::move(n)), health(h), current_stamina(1), stamina(s), current_mana(1), mana(m), strength(str), dex(d), luck(l), weapon(w), armor(a), battlefield(b) {}

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
        stats();
        current_stamina = std::min(stamina, current_stamina + stamina_restore);
        current_mana = std::min(mana, current_mana + mana_restore);
    }

    virtual void reduce_distance();

    virtual void increase_distance();

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
            {"szybkie ciecie", Attack<T>(magical, 0, 5, 2, 2, 0, 2)},
            {"daleki atak", Attack<T>(magical,5, 7, 1, 6, 2, 6)},
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



template <SubAddMult T>
class WomKnight : public Wombat<T>{
public:
    WomKnight(std::string n, ColdWeapon<T>* w, HeavyArmor<T>* a, Battlefield<T>& b) : Wombat<T>(n, 50, 10, 5, 5, 4, 2, w, a, b) {}

    void passive() override{
        this->reduce_distance();
    }

    void reduce_distance() override;

};

template <SubAddMult T>
class WoArcher : public Wombat<T>{
public:
    WoArcher(std::string n, DistanceWeapon<T>* w, LightArmor<T>* a, Battlefield<T>& b) : Wombat<T>(n, 45, 7, 10, 2, 6, 3, w, a, b) {}

    void increase_distance() override;

};

template <SubAddMult T>
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


    template <WeaponArmor<T> U>
    U choose_item(std::map<std::string, U> items){
        for (const auto& elem : items) {
            std::cout << elem.first << std::endl;
        }
        while(true){
            std::string chosen;
            std::getline(std::cin, chosen);
            std::cout << std::endl;
            if(items.contains(chosen)){
                std::cout << "Wybrales: " << chosen << std::endl << std::endl;
                return items.at(chosen);
            } else {
                std::cout << "Niestety item: " << chosen << " nie istnieje, sprobuj jeszcze raz" << std::endl << std::endl;

            }
        }

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
                        std::cout << "Wybrales rycerza" << std::endl;


                        std::map<std::string, Weapon<T>*> weapons{
                                {"Falchion", new Falchion<T>()},
                                {"Morgenstern", new Morgenstern<T>()},
                                {"Koncerz", new Koncerz<T>()}
                        };
                        std::cout << "Wybierz bron: " << std::endl;
                        auto* weapon = static_cast<ColdWeapon<T> *>(this->choose_item(weapons));

                        std::map<std::string, Armor<T>*> armors{
                                {"Zbroja Havla", new HavelArmor<T>()},
                                {"Zbroja Elity Rycerstwa", new EliteKnightArmor<T>()}
                        };

                        std::cout << "Wybierz ciezki pancerz: " << std::endl;
                        auto* armor = static_cast<HeavyArmor<T> *>(this->choose_item(armors));
                        return new WomKnight(name, weapon, armor, *this);
                    }
                    case 1: {
                        std::cout << "Wybrales Lucznika" << std::endl;

                        std::map<std::string, Weapon<T>*> weapons{
                                {"Luk", new Bow<T>()},
                                {"Proca", new Slingshot<T>()}
                        };

                        std::cout << "Wybierz bron: " << std::endl;
                        DistanceWeapon<T>* weapon = static_cast<DistanceWeapon<int> *>(this->choose_item(weapons));

                        std::cout << "Wybierz lekki pancerz: " << std::endl;

                        std::map<std::string, Armor<T>*> armors{
                                {"Zbroja Zlodzieja", new ThiefArmor<T>()},
                                {"Zbroja Lowcy", new HunterArmor<T>()}
                        };

                        auto* armor = static_cast<LightArmor<T> *>(this->choose_item(armors));
                        return new WoArcher(name, weapon, armor, *this);
                    }

                }

            } else {
                std::cout << "Prosze podac ponownie, klasa " << chosen_class << " jest niepoprawna"<< std::endl;
            }

        }

    }

    void turn(Wombat<T>* first, Wombat<T>* second){
        std:: cout << "------------------------------------------------" << std:: endl;
        std::cout << "Tura " << first->name << std::endl;
        std::cout << "Obecny dystans wynosi: " << current_distance << std::endl;

        first->choose_move(*second, current_distance);

    }

    void game(){
        std::cout << "-------PLAYER 1------" << std::endl;
        Wombat<T>* player1 = create_wombat("Player 1");

        std::cout << "-------PLAYER 2------" << std::endl;
        Wombat<T>* player2 = create_wombat("Player 2");


        std::cout << "-------FIGHT------" << std::endl;
        Wombat<T>* first, *second;

        std::tie(first, second) = (*player1 > *player2) ? std::make_pair(player1, player2) : std::make_pair(player2, player1);

        while(true){
            turn(first, second);

            if(second->is_dead()){
                std::cout << "Wygral gracz: " << first->name << std::endl;
                break;
            }

            turn(second, first);

            if(first->is_dead()){
                std::cout << "Wygral gracz: " << second->name << std::endl;
                break;
            }
        }


    }
};

template <SubAddMult T>
inline void Wombat<T>::reduce_distance() {
    battlefield.reduce_distance(dex);
}

template <SubAddMult T>
inline void Wombat<T>::increase_distance() {
    battlefield.increase_distance(dex);
}


template <SubAddMult T>
inline void WomKnight<T>::reduce_distance() {
    this->battlefield.reduce_distance(this->dex * 2);
}

template <SubAddMult T>
inline void WoArcher<T>::increase_distance() {
    this->battlefield.increase_distance(this->dex * 2);
}


template <SubAdd T>
inline void HunterArmor<T>::effect(Wombat<T> &wombat) {
    wombat.dex += 5;
    wombat.current_stamina = std::min(wombat.stamina, wombat.stamina + 5);
}

template <SubAdd T>
inline void ThiefArmor<T>::effect(Wombat<T> &wombat) {
    wombat.dex += 15;
    wombat.health = std::max(wombat.health - 10, 1);
    wombat.current_stamina = std::min(wombat.stamina, wombat.stamina + 5);
}


template <SubAdd T>
inline void HavelArmor<T>::effect(Wombat<T> &wombat) {
    wombat.dex = std::max(0, wombat.dex - 5);
    wombat.strength += 4;
    wombat.stamina = std::max(0, wombat.stamina - 5);
}

template <SubAdd T>
inline void EliteKnightArmor<T>::effect(Wombat<T> &wombat) {
    wombat.dex = std::max(0, wombat.dex - 4);
    wombat.health += 15;
    wombat.stamina = std::max(0, wombat.stamina - 5);
}

int main() {
    Battlefield<int> battlefield;
    battlefield.game();
}
