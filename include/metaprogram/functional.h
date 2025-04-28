#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <iostream>

#define BY_NAME(F)        \
    [](auto&& ...arg){    \
        return F(arg...); \
    }                     \

template<typename F,class...Args>
auto foo(F f,Args&&...args) {
    return f(std::forward<Args>(args)...);
}

#endif