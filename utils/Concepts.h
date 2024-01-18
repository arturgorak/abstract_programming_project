#pragma once

#include <type_traits>

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
concept SubAdd  = requires (T a) { Add<T> && Sub<T>; };

template <typename T>
concept Mult = requires ( T a , T b ) {
    a*b;
};

template <typename T>
concept SubAddMult  = requires (T a) { Add<T> && Sub<T> && Mult<T>; };

