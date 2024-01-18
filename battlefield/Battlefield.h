#include "../wombats/WombatClasses.h"

#include <ranges>



template <typename T, typename U>
concept WeaponArmor = (std::is_same_v<T, Armor<U>*> || std::is_same_v<T, Weapon<U>*>) && SubAddMult<U>;

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

    int get_current_distance(){
        return current_distance;
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

        delete player1;
        delete player2;


    }
};

