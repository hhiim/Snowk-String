#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <concepts>
#include <type_traits>

struct nil{};

template<bool T> struct True;
template<> struct True<true> {
    const bool value = true;
};

template<bool T> struct False;
template<> struct False<false> {
    const bool value = false;
};

template<typename T>
concept Tt = std::same_as<T, True<true>>;
template<typename F>
concept Ft = std::same_as<F, False<false>>;

#define Bool static const bool
#define CHECK_t(X) using check = True<(X)>
#define CHECK_f(X) using check = False<(X)>
#define CHECK(X) (typename X::check){}

constexpr bool IF(bool p, bool q){
    return !p || q;
}

#endif