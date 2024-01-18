cmake_minimum_required(VERSION 3.26)
project(wombat)

set(CMAKE_CXX_STANDARD 23)

add_executable(wombat main.cpp
        utils/Concepts.h
        wombats/Wombat.h
        battlefield/Battlefield.h
        equipment/Armor.h
        equipment/Weapon.h
        equipment/Attack.h
        wombats/WombatClasses.h)

add_subdirectory(tests)