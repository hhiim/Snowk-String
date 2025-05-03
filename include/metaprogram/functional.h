#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <functional>
#include <tuple>

#define BY_NAME(F)        \
    [](auto&& ...arg){    \
        return F(arg...); \
    }                     \

template<typename F,class...Args>
auto foo(F f,Args&&...args) {
    return f(std::forward<Args>(args)...);
}


template <typename T>
struct funcTraits;

// 普通函数
template <typename R, typename... Args>
struct funcTraits<R(Args...)> {
    using ReturnType = R;
    using ArgsTuple = std::tuple<Args...>;
    static constexpr size_t ArgCount = sizeof...(Args);
};

// 成员函数（非 const）
template <typename R, typename C, typename... Args>
struct funcTraits<R (C::*)(Args...)> {
    using ReturnType = R;
    using ClassType = C;
    using ArgsTuple = std::tuple<Args...>;
    static constexpr size_t ArgCount = sizeof...(Args);
};

// 成员函数（const）
template <typename R, typename C, typename... Args>
struct funcTraits<R (C::*)(Args...) const> :
    funcTraits<R (C::*)(Args...)> {};

#endif