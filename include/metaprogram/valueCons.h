#ifndef VALUE_CONS_H
#define VALUE_CONS_H

#include <concepts>
#include <cstddef>
#include <type_traits>
#include "constraint.h"

#define Constexpr static constexpr auto

namespace Dstring::meta{

// cons 定义
template <auto A, typename B>
struct vcons { };

template<typename T>
struct consValues;
template<auto X, typename Y>
struct consValues<vcons<X, Y>> {
    Constexpr A = X;
    using B = Y;
};

// List 概念与类型萃取
template<typename T>
struct isVList;

template<>
struct isVList<nil>
{ CHECK_t(true); };

template<typename T>
concept VList = requires {
    CHECK(isVList<T>);
};

template<auto X, typename Y>
struct isVList<vcons<X, Y>> {
    using T = vcons<X, Y>;
    Bool checkB =  VList<Y>;
    CHECK_t(checkB);
};

// List 操作
template <auto ...Args> struct createVCons;
template <auto T, auto ...Args>
struct createVCons<T, Args...>{
    using result = vcons<T, typename createVCons<Args...>::result>;
};
template <> struct createVCons<>{
    using result = nil;
};

template <VList X, VList Y> struct Vconcat;
template <VList Y> struct Vconcat<Y, nil>{
    using result = Y;
};
template <auto X, VList Y, VList Z>
struct Vconcat<vcons<X, Y>, Z>{
    using result = vcons<X, Vconcat<Y, Z>>;
};


template<VList X, auto Y> struct Vappend;
template<auto Y> struct Vappend<nil, Y> {
    using result = vcons<Y, nil>;
};
template<auto head, VList tail, auto Y>
struct Vappend<vcons<head, tail>, Y>{
    using result = vcons<head,
        typename Vappend<tail, Y>::result>;
};

template<VList X> struct Vreverse;
template<> struct Vreverse<nil>{
    using result = nil;
};
template<auto head, VList tail>
struct Vreverse<vcons<head, tail>>{
    using T = Vreverse<tail>::result;
    using result = Vappend<T,head>::result;
};

template<auto F, VList X> struct Vmap;
template<auto F> struct Vmap<F, nil>{ using result = nil; };
template< auto F, auto head, VList tail>
struct Vmap<F, vcons<head, tail>>{
    using result = vcons<F(head),
        typename Vmap<F, tail>::result>;
};

template<VList L, auto T>
using Vpush = Vappend<L, T>;

template<VList L> struct Vpop;
template<auto T> struct Vpop<vcons<T, nil>>{
    using result = nil;
};
template<auto T, VList L> struct Vpop<vcons<T, L>>{
    using result = vcons<T, typename Vpop<L>::result>;
};

template<VList L> struct VstackTop;
template<auto T> struct VstackTop<vcons<T, nil>>
{  Constexpr result = T;  };
template<auto T, VList L> struct VstackTop<vcons<T, L>>
{  Constexpr result = VstackTop<L>::result;  };


template<VList L, size_t N = 0> struct Vget;
template<auto T, VList L>
struct Vget<vcons<T, L>,0> {  Constexpr result = T;  };
template<auto T, VList L, size_t N>
struct Vget<vcons<T, L>,N>
{  Constexpr result = Vget<L, N-1>::result; };

template <auto ...Args> struct Vpack{};

namespace {
    template<auto ...Args>
    void h(Vpack<Args...> _){}
};

template <typename U>
concept isVpack = requires(U x) { h(x); };


template <VList L, auto ...Args> struct Vlist2pack_impl;
template <auto ...Args>
struct Vlist2pack_impl<nil, Args...>{
    using result = Vpack<Args...>;
};
template <auto X, VList L, auto ...Args>
struct Vlist2pack_impl<vcons<X, L>, Args...>{
    using result = Vlist2pack_impl<L, X, Args...>::result;
};
template <VList L, auto ...Args> 
using Vlist2pack = Vlist2pack_impl
    <typename Vreverse<L>::result, Args...>;

};

#endif