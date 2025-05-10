#ifndef TYPE_CONS_H
#define TYPE_CONS_H

#include <concepts>
#include <cstddef>
#include <type_traits>
#include "constraint.h"

namespace Dstring::meta{
    
// cons 定义
template <typename A, typename B>
struct tcons { };

template<typename T>
struct consTypes;
template<typename X, typename Y>
struct consTypes<tcons<X, Y>> {
    using A = X;
    using B = Y;
};

template<typename T> using First  = consTypes<T>::A;
template<typename T> using Second = consTypes<T>::B;

template<typename T>
concept tCons = requires {
    typename consTypes<T>::A;
    typename consTypes<T>::B;
};

// List 概念与类型萃取
template<typename T>
struct isTList;

template<>
struct isTList<nil>
{ CHECK_t(true); };

template<typename T>
concept TList = requires {
    CHECK(isTList<T>);
};

template<typename X, typename Y>
struct isTList<tcons<X, Y>> {
    using T = tcons<X, Y>;
    Bool checkA =  !std::same_as<nil, X>;
    Bool checkB =  TList<Y>;
    CHECK_t(checkA && checkB);
};

// List 操作
template <typename ...Args> struct createTCons;
template <typename T, typename ...Args>
struct createTCons<T, Args...>{
    using result = tcons<T, typename createTCons<Args...>::result>;
};
template <> struct createTCons<>{
    using result = nil;
};

template <TList X, TList Y> struct Tconcat;
template <TList Y> struct Tconcat<Y, nil>{
    using result = Y;
};
template <typename X, TList Y, TList Z>
struct Tconcat<tcons<X, Y>, Z>{
    using result = tcons<X, Tconcat<Y, Z>>;
};


template<TList X, typename Y> struct Tappend;
template<typename Y> struct Tappend<nil, Y> {
    using result = tcons<Y, nil>;
};
template<typename head, TList tail, typename Y>
struct Tappend<tcons<head, tail>, Y>{
    using result = tcons<head,
        typename Tappend<tail, Y>::result>;
};

template<TList X> struct Treverse;
template<> struct Treverse<nil>{
    using result = nil;
};
template<typename head, TList tail>
struct Treverse<tcons<head, tail>>{
    using T = Treverse<tail>::result;
    using result = Tappend<T,head>::result;
};

template<template<typename T> class F, TList X>
struct Tmap;
template<template<typename T> class F>
struct Tmap<F, nil>{ using result = nil; };
template<
    template<typename T> class F,
    typename head, TList tail
> struct Tmap<F, tcons<head, tail>>{
    using result = tcons<F<head>,
        typename Tmap<F, tail>::result>;
};

template<TList L, typename T>
using Tpush = Tappend<L, T>;

template<TList L> struct Tpop;
template<typename T> struct Tpop<tcons<T, nil>>{
    using result = nil;
};
template<typename T, TList L> struct Tpop<tcons<T, L>>{
    using result = tcons<T, typename Tpop<L>::result>;
};

template<TList L> struct TstackTop;
template<typename T> struct TstackTop<tcons<T, nil>>
{  using result = T;  };
template<typename T, TList L> struct TstackTop<tcons<T, L>>
{  using result = TstackTop<L>::result;  };


template<TList L, size_t N = 0> struct Tget;
template<typename T, TList L>
struct Tget<tcons<T, L>,0> {  using result = T;  };
template<typename T, TList L, size_t N>
struct Tget<tcons<T, L>,N> {  using result = Tget<L, N-1>;  };


};

#endif