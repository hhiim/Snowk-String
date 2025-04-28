#ifndef STATIC_CAST_H
#define STATIC_CAST_H

#include <concepts>

template<typename T>
concept callable = requires(T t){ t(); };

template <typename T, typename U>
concept StaticCastable = requires(T t) {
    {t.template staticCast<U>()} ->
        std::convertible_to<U>;
};

// 简单实现重载 static_cast
template <typename T>
auto _static_cast(auto&& obj){
    if constexpr (StaticCastable<decltype(obj),T>)
        return obj.template staticCast<T>();
    else return static_cast<T>(obj);
}

# define static_cast _static_cast
#endif