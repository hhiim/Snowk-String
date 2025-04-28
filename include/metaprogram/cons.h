#ifndef CONS_H
#define CONS_H

#include <concepts>
#include <type_traits>
#include "constraint.h"

// cons 定义
struct nil{};
template <typename A, typename B>
struct cons { A a; B b; };

template<typename T>
struct consTypes;
template<typename X, typename Y>
struct consTypes<cons<X, Y>> {
    using A = X;
    using B = Y;
};

template<typename T> using First  = consTypes<T>::A;
template<typename T> using Second = consTypes<T>::B;

template<typename T>
concept Cons = requires {
    typename consTypes<T>::A;
    typename consTypes<T>::B;
};

// List 概念与类型萃取
template<typename T>
struct isList;

template<>
struct isList<nil>
{ CHECK_t(true); };

template<typename T>
concept List = requires {
    CHECK(isList<T>);
};

template<typename X, typename Y>
struct isList<cons<X, Y>> {
    using T = cons<X, Y>;
    Bool checkA =  !std::same_as<nil, X>;
    Bool checkB =  List<Y>;
    CHECK_t(checkA && checkB);
};


#endif